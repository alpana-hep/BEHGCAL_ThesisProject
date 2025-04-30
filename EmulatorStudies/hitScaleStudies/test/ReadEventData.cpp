#include "TDirectory.h"
#include "TFile.h"
#include "TObjString.h"
#include "TTree.h"
#include "EventData.h"
#include <cstdint>
#include <string>
#include <map>
#include <vector>
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"

struct TruncationEvent {
  uint64_t beginBx = 0;
  uint64_t endBx = 0;
  std::vector<double> locall1a_begin;
  unsigned nEvents = 0;
  unsigned nTruncEvents = 0;
  unsigned nTruncatedEconPackets = 0;

  void Print() const {
    std::cout << "TruncationEvent: [" << beginBx << ", " << endBx << "], localL1A = " << locall1a_begin.at(5) << " kHz, ";
    std::cout << "evts = " << nEvents << ", truncEvts = " << nTruncEvents << "\n";
  }
};

struct TimeSeriesAnalysis {
  std::vector<uint64_t> l1a_bx;
  std::vector<TruncationEvent> events;

  // Return the L1 rate in the last N BX in kHz
  unsigned getNPrevious(unsigned idx, unsigned N) {
    unsigned n_l1a = 0;
    for (int scan_idx = idx - 1; scan_idx >= 0; --scan_idx) {
      unsigned delta = l1a_bx[idx] - l1a_bx[scan_idx];
      if (delta > N) {
        break;
      } else {
        n_l1a += 1;
      }
    }
    return n_l1a;
  }

  double localL1ARate(unsigned idx, unsigned N) {
    return 40E3 * double(getNPrevious(idx, N)) / double(N);
  }
};


int main(int argc, char *argv[]) {
  TH1::AddDirectory(false);


  assert(argc >= 3);
  std::string file_in = argv[1];
  std::string file_out = argv[2];

  std::map<std::string, int> int_vals;
  std::map<std::string, double> float_vals;

  TFile f(file_in.c_str());

  TObjString * cfg =dynamic_cast<TObjString*>(gDirectory->Get("config"));

  TTree *t = dynamic_cast<TTree*>(gDirectory->Get("CentralDaq"));
  if (t == nullptr) {
    std::cout << "ROOT file does not contain CentralDaq tree\n";
    return 1;
  }
  EventData *edata = new EventData();
  t->SetBranchAddress("EventData", &edata);

  unsigned entries = t->GetEntries();
  unsigned long tot_bytes = t->GetTotBytes();
  unsigned long zip_bytes = t->GetZipBytes();

  std::cout << "Entries: " << entries << "\n";
  std::cout << "Uncompressed: " << double(tot_bytes) / double(entries) << " bytes / event\n";
  std::cout << "Compressed: " << double(zip_bytes) / double(entries) << " bytes / event\n";

  unsigned n_slinks = 0;
  TH1D * nEvents = new TH1D("nEvents", "", 1, 0, 1);
  TH1D * nEcondTruncations = new TH1D("nEcondTruncations", "", 1, 0, 1);
  TH1D * nBETruncations = new TH1D("nBETruncations", "", 1, 0, 1);
  TH2D * n2DEcondTruncationsPerSlink = nullptr;
  TH2D * n2DBETruncationsPerSlink = nullptr;
  TH1D * n1DEcondTruncationsPerSlink = nullptr;
  TH1D * n1DBETruncationsPerSlink = nullptr;
  TH1I * nEcondTruncPackets = new TH1I("nEcondTruncPackets", "", 4000, -0.5, 4000 - 0.5);
  TH1I * nBETruncPackets = new TH1I("nBETruncPackets", "", 4000, -0.5, 4000 - 0.5);
  TH1D * nEcondTruncClusters = new TH1D("nEcondTruncClusters", "", 1, 0, 1);
  TH1D * econdClusterSize = new TH1D("econdClusterSize", "", 100, -0.5, 99.5);

  std::vector<int> l1a_windows = {100, 200, 500, 1000, 2000, 5000, 10000};
  std::vector<TH1D*> localL1A_all;
  std::vector<TH1D*> localL1A_econdtrunc;
  std::vector<TH1D*> localL1A_clusters;
  for (auto l1a_window : l1a_windows) {
    localL1A_all.emplace_back(new TH1D(TString::Format("localL1A_all_%i", l1a_window), "", 100, 300., 1500.));
    localL1A_econdtrunc.emplace_back(new TH1D(TString::Format("localL1A_econdtrunc_%i", l1a_window), "", 100, 300., 1500.));
    localL1A_clusters.emplace_back(new TH1D(TString::Format("localL1A_clusters_%i", l1a_window), "", 100, 300., 1500.));
  }

  TimeSeriesAnalysis time_ana;
  time_ana.l1a_bx.resize(entries);

  bool open_cluster = false;

  for (unsigned ievt = 0; ievt < entries; ++ievt) {
    t->GetEntry(ievt);

    if (ievt == 0) {
      n_slinks = edata->slinkDataVector().size();
      n2DEcondTruncationsPerSlink = new TH2D("n2DEcondTruncationsPerSlink", "", n_slinks, -0.5, -0.5 + n_slinks,  n_slinks, -0.5, -0.5 + n_slinks);
      n2DBETruncationsPerSlink = new TH2D("n2DBETruncationsPerSlink", "", n_slinks, -0.5, -0.5 + n_slinks,  n_slinks, -0.5, -0.5 + n_slinks);
      n1DEcondTruncationsPerSlink = new TH1D("n1DEcondTruncationsPerSlink", "", n_slinks, -0.5, -0.5 + n_slinks);
      n1DBETruncationsPerSlink = new TH1D("n1DBETruncationsPerSlink", "", n_slinks, -0.5, -0.5 + n_slinks);
    }

    auto slinkboe = edata->slinkDataVector().at(0).slinkBoe();

    // Skip the first orbit to make sure the system has warmed up
    if (slinkboe.counters().oc() == 0) {
      continue;
    }

    nEvents->Fill(0.5);

    uint64_t thisBx = slinkboe.counters().totalBx();
    time_ana.l1a_bx[ievt] = thisBx;
    std::vector<double> locall1a(l1a_windows.size());
    for (unsigned iw = 0; iw < l1a_windows.size(); ++iw) {
      locall1a[iw] = time_ana.localL1ARate(ievt, l1a_windows[iw]);
      localL1A_all[iw]->Fill(locall1a[iw]);
    }
    // How to define a truncation event?
    // Cluster - connect two events that are separated by less than X BX.
    // Then a truncation event is defined by any contiguous set of connected events.
    if (open_cluster) {
      time_ana.events.back().nEvents += 1;
    }
    if (edata->numberOfEcondAsicTruncations() > 0) {
      // Do I need to close the last cluster?
      if (open_cluster && (thisBx - time_ana.events.back().endBx) > 500) {
        open_cluster = false;
        std::cout << "CLUSTER CLOSED\n";
        time_ana.events.back().Print();
        for (unsigned iw = 0; iw < l1a_windows.size(); ++iw) {
          localL1A_clusters[iw]->Fill(time_ana.events.back().locall1a_begin[iw]);
        }
        nEcondTruncClusters->Fill(0.5);
        econdClusterSize->Fill(time_ana.events.back().nTruncEvents);
      }

      // Is current cluster closed? Open it...
      if (!open_cluster) {
        time_ana.events.push_back(TruncationEvent());
        open_cluster = true;
        time_ana.events.back().beginBx = thisBx;
        time_ana.events.back().locall1a_begin.resize(l1a_windows.size());
        time_ana.events.back().locall1a_begin = locall1a;
        std::cout << "CLUSTER OPENED\n";
      }

      time_ana.events.back().nTruncEvents += 1;
      time_ana.events.back().nTruncatedEconPackets += edata->numberOfEcondAsicTruncations();
      time_ana.events.back().endBx = thisBx;

      for (unsigned iw = 0; iw < l1a_windows.size(); ++iw) {
        localL1A_econdtrunc[iw]->Fill(locall1a[iw]);
      }
      // std::cout << ievt << "\t" << edata->numberOfEcondAsicTruncations() << "\t BX = " << thisBx << "\t LocalL1ARate = " << locall1a.at(5) << "\n";
      // slinkboe.print();
    }
    nEcondTruncPackets->Fill(edata->numberOfEcondAsicTruncations());
    nBETruncPackets->Fill(edata->numberOfEcondBeTruncations());

    if (edata->numberOfEcondAsicTruncations() > 0) {
      nEcondTruncations->Fill(0.5);
    }
    if (edata->numberOfEcondBeTruncations() > 0) {
      nBETruncations->Fill(0.5);
    }
    edata->numberOfEcondAsicTruncations();
    for (unsigned is = 0; is < n_slinks; ++is) {
      if (edata->slinkDataVector()[is].numberOfEcondAsicTruncations() > 0) {
        n1DEcondTruncationsPerSlink->Fill(is);
      }
      if (edata->slinkDataVector()[is].numberOfEcondBeTruncations() > 0) {
        n1DBETruncationsPerSlink->Fill(is);
      }

      // Skip doing the 2D for now

      // for (unsigned js = 0; js < n_slinks; ++js) {
      //   if (edata->slinkDataVector()[is].numberOfEcondAsicTruncations() > 0 && edata->slinkDataVector()[js].numberOfEcondAsicTruncations() > 0) {
      //     n2DEcondTruncationsPerSlink->Fill(is, n_slinks - js - 1);
      //   }
      //   if (edata->slinkDataVector()[is].numberOfEcondBeTruncations() > 0 && edata->slinkDataVector()[js].numberOfEcondBeTruncations() > 0) {
      //     n2DBETruncationsPerSlink->Fill(is, n_slinks - js - 1);
      //   }
      // }
    }
  }

  TFile fout(file_out.c_str(), "RECREATE");
  fout.cd();
  cfg->Write("config");
  nEvents->Write();
  nEcondTruncations->Write();
  nBETruncations->Write();
  n2DEcondTruncationsPerSlink->Write();
  n2DBETruncationsPerSlink->Write();
  n1DEcondTruncationsPerSlink->Write();
  n1DBETruncationsPerSlink->Write();
  nEcondTruncPackets->Write();
  nBETruncPackets->Write();
  for (unsigned iw = 0; iw < l1a_windows.size(); ++iw) {
    std::cout << l1a_windows[iw] << "\t" << localL1A_all[iw]->GetMean() << "/" << localL1A_all[iw]->GetRMS() 
      << "\t" << localL1A_econdtrunc[iw]->GetMean() << "/" << localL1A_econdtrunc[iw]->GetRMS()
      << "\t" << localL1A_clusters[iw]->GetMean() << "/" << localL1A_clusters[iw]->GetRMS() << "\n";
    localL1A_all[iw]->Write();
    localL1A_econdtrunc[iw]->Write();
    localL1A_clusters[iw]->Write();
  }
  nEcondTruncClusters->Write();
  econdClusterSize->Write();
  // localL1A_all->Write();
  // localL1A_econdtrunc->Write();
  fout.Close();
  return 0;
}
