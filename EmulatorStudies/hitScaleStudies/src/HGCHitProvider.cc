#include "HGCHitProvider.h"
#include "EcondAsicHitCounts.h"
#include "Definer.h"
#include "Utils.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

#include "TFile.h"
#include "json.hpp"

using json = nlohmann::json;

HGCHitProvider::HGCHitProvider() : read_unmasked_(true), current_evt_(0) { ; }

HGCHitProvider::~HGCHitProvider() {
  if (t_ != nullptr) {
    delete t_;
  }
}

void HGCHitProvider::initialise(std::string const& mapping, std::string const& config) {
  std::vector<EcondAsicDefinition> defs;
  if (ends_with(mapping, ".json")) {
    Definer def;
    def.readFile(mapping);
    std::vector<LpgbtDefinition> const& vLd(def.lpgbtDefinitionVector());
    for (unsigned i = 0; i < vLd.size(); i++) {
      vLd[i].econdAsicDefinitions(defs);
    }
  } else {
    std::ifstream infile(mapping);
    std::string line;
    while (std::getline(infile, line)) {
      std::istringstream iss(line);
      int layer;
      int u;
      int v;
      int type;
      char shape;
      if (!(iss >> layer >> u >> v >> type >> shape)) {
        break;
      }
      EcondAsicDefinition def;
      // std::cout << layer << " " << u << " " << v << " " << type << " " << shape << "\n";
      def.setDefinitionTo(layer, u, v, type, shape, 0);
      defs.push_back(def);
    }
  }
  std::sort(defs.begin(), defs.end());
  this->initialise(defs, config);
}

void HGCHitProvider::initialise(std::vector<EcondAsicDefinition> const& defs,
                                std::string const& config) {
  econ_defs_ = defs;
  mods_.clear();
  orig_hits_.clear();
  trans_hits_.clear();
  trans_.clear();

  std::ifstream test(config, std::ifstream::binary);
  json x = json::parse(test);
  unsigned N = defs.size();
  mods_.resize(N);
  orig_hits_.resize(N);
  trans_hits_.resize(N);
  trans_.resize(N);

  unsigned cfg_idx = 0;
  Module cmod;
  cmod.layer = x[cfg_idx]["key"][0];
  cmod.u = x[cfg_idx]["key"][1];
  cmod.v = x[cfg_idx]["key"][2];

  for (unsigned i = 0; i < N; ++i) {
    Module dmod;
    dmod.layer = defs[i].layer();
    dmod.type = defs[i].type();
    dmod.u = defs[i].u();
    dmod.v = defs[i].v();
    dmod.s = defs[i].shape();

    while (cmod < dmod && cfg_idx < (x.size() - 1)) {
      // std::cout << "Behind: Mine" << cmod << ", Defs" << dmod << "\n";
      ++cfg_idx;
      cmod.layer = x[cfg_idx]["key"][0];
      cmod.u = x[cfg_idx]["key"][1];
      cmod.v = x[cfg_idx]["key"][2];
    }

    if (dmod == cmod) {
      // std::cout << "MATCH: Mine" << cmod << ", Defs" << dmod << "\n";

      mods_[i] = dmod;
      if (mods_[i].type == 0) {
        mods_[i].hdm = true;
      } else {
        mods_[i].hdm = false;
      }
      std::string transtype = x[cfg_idx]["TransformMethod"];
      trans_[i].ScaleN30 = x[cfg_idx]["ScaleN30"];
      if (transtype == "Zero") {
        trans_[i].type = 0;
      } else if (transtype == "NoTransform") {
        trans_[i].type = 1;
      } else if (transtype == "FixedNumber") {
        trans_[i].type = 2;
        trans_[i].FixedNumberMax = x[cfg_idx]["FixedNumberMax"];
      } else if (transtype == "FixedOccupancy") {
        trans_[i].type = 3;
        trans_[i].FixedOccupancyFrac = x[cfg_idx]["FixedOccupancyFrac"];
      } else if (transtype == "FixedDensity") {
        trans_[i].type = 4;
        trans_[i].FixedDensitySrc = x[cfg_idx]["FixedDensitySrc"];
        trans_[i].FixedDensityTarget = x[cfg_idx]["FixedDensityTarget"];
      } else if (transtype == "FixedDensity2") {
        trans_[i].type = 5;
        trans_[i].FixedDensitySrc = x[cfg_idx]["FixedDensitySrc"];
        trans_[i].FixedDensityTarget = x[cfg_idx]["FixedDensityTarget"];
        trans_[i].FixedDensityExponentTotal = x[cfg_idx]["FixedDensityExponentTotal"];
        trans_[i].FixedDensityExponentTOA = x[cfg_idx]["FixedDensityExponentTOA"];
        trans_[i].FixedDensityExponentBXM1 = x[cfg_idx]["FixedDensityExponentBXM1"];
      }
      // Keep a record of the max allowed number of hits
    
      trans_[i].nCells = x[cfg_idx].value("n_cells", 0);

      if (cfg_idx < (x.size() - 1)) {
        ++cfg_idx;
        cmod.layer = x[cfg_idx]["key"][0];
        cmod.u = x[cfg_idx]["key"][1];
        cmod.v = x[cfg_idx]["key"][2];
      }
    } else {
      // std::cout << "Mismatch: Mine" << cmod << ", Defs" << dmod << "\n";
    }
  }
}
void HGCHitProvider::processEvent(unsigned long ievt) {
  evt_.GetEntry(ievt);
  unsigned my_idx = 0;
  unsigned tN = evt_.layer->size();
  Module tmod;

  // Reset
  for (unsigned idx = 0; idx < mods_.size(); ++idx) {
    orig_hits_[idx].nhits10 = 0;
    orig_hits_[idx].nhits20 = 0;
    orig_hits_[idx].nhits30 = 0;
    orig_hits_[idx].nhitsBXM1 = 0;
    trans_hits_[idx].nhits10 = 0;
    trans_hits_[idx].nhits20 = 0;
    trans_hits_[idx].nhits30 = 0;
    trans_hits_[idx].nhitsBXM1 = 0;
  }

  for (unsigned t_idx = 0; t_idx < tN; ++t_idx) {
    tmod.layer = (*evt_.layer)[t_idx];
    tmod.type = (*evt_.type)[t_idx];
    tmod.u = (*evt_.wafer_u)[t_idx];
    tmod.v = (*evt_.wafer_v)[t_idx];

    // Am I behind?
    while (mods_[my_idx] < tmod && my_idx < (mods_.size() - 1)) {
      // std::cout << "Behind: Mine" << mods_[my_idx] << ", Tree" << tmod << "\n";
      ++my_idx;
    }

    std::vector<int>* read_nhits10 = evt_.nhits10_defined;
    std::vector<int>* read_nhits20 = evt_.nhits20_defined;
    std::vector<int>* read_nhits30 = evt_.nhits30_defined;
    std::vector<int>* read_nhitsBXM1 = evt_.nhitsBXM1_defined;
    // if (read_unmasked_) {
    //   read_nhits10 = evt_.nhits10_unmasked;
    //   read_nhits20 = evt_.nhits20_unmasked;
    //   read_nhits30 = evt_.nhits30_unmasked;
    //   read_nhitsBXM1 = evt_.nhitsBXM1_unmasked;
    // }
    if (tN != read_nhits10->size() || tN != read_nhits20->size() || tN != read_nhits30->size() ){ //|| tN != read_nhitsBXM1->size()) {
      throw std::runtime_error("Error in input ROOT file: vectors containing hit values have unequal lengths");
    }

    if (tmod == mods_[my_idx]) {
      // Backwards compatibility for old ntuples without nhits10
      if (evt_.nhits10_defined != nullptr) {
        orig_hits_[my_idx].nhits10 = (*read_nhits10)[t_idx];
      } else {
        orig_hits_[my_idx].nhits10 = 0;
      }
      orig_hits_[my_idx].nhits20 = (*read_nhits20)[t_idx];
      orig_hits_[my_idx].nhits30 = (*read_nhits30)[t_idx];
      orig_hits_[my_idx].nhitsBXM1 = (*read_nhitsBXM1)[t_idx];
      // std::cout << "MATCH: Mine" << mods_[my_idx] << ", Tree" << tmod << "\n";
      if (my_idx < (mods_.size() - 1)) {
        ++my_idx;
      }

    } else {
      // std::cout << "Mismatch: Mine" << mods_[my_idx] << ", Tree" << tmod << "\n";
    }
  }

  for (unsigned idx = 0; idx < mods_.size(); ++idx) {
    double n_src_tot =
        double(orig_hits_[idx].nhits10 + orig_hits_[idx].nhits20 + orig_hits_[idx].nhits30);
    double n_tgt_tot = 0.;
    double n_tgt_30 = 0.;
    double n_tgt_bxm1 = 0.;
    if (trans_[idx].type == 1) {
      n_tgt_tot = orig_hits_[idx].nhits10 + orig_hits_[idx].nhits20 + orig_hits_[idx].nhits30;
    } else if (trans_[idx].type == 2) {
      int n_current_tot =
          orig_hits_[idx].nhits10 + orig_hits_[idx].nhits20 + orig_hits_[idx].nhits30;
      if (n_current_tot <= trans_[idx].FixedNumberMax) {
        n_tgt_tot = n_src_tot;
      } else {
        n_tgt_tot = double(trans_[idx].FixedNumberMax);
      }
    } else if (trans_[idx].type == 3) {
      double f_scale = trans_[idx].FixedOccupancyFrac;
      n_tgt_tot =
          double(orig_hits_[idx].nhits10 + orig_hits_[idx].nhits20 + orig_hits_[idx].nhits30) *
          f_scale;
    } else if (trans_[idx].type == 4) {
      double src = trans_[idx].FixedDensitySrc;
      double tgt = trans_[idx].FixedDensityTarget;
      if (src <= 0. || tgt <= 0.) {
        throw std::runtime_error("FixedDensity conversion requested with src <= 0 or tgt <= 0");
      }
      n_tgt_tot = tgt * (1. - std::pow((src - n_src_tot) / src, src / tgt));
    } else if (trans_[idx].type == 5) {
      double src = trans_[idx].FixedDensitySrc;
      double tgt = trans_[idx].FixedDensityTarget;
      if (src <= 0. || tgt <= 0.) {
        throw std::runtime_error("FixedDensity2 conversion requested with src <= 0 or tgt <= 0");
      }
      n_tgt_tot =
          tgt * (1. - std::pow((src - n_src_tot) / src, trans_[idx].FixedDensityExponentTotal));
      n_tgt_30 = tgt * (1. - std::pow((src - double(orig_hits_[idx].nhits30)) / src,
                                      trans_[idx].FixedDensityExponentTOA));
      n_tgt_bxm1 = tgt * (1. - std::pow((src - double(orig_hits_[idx].nhitsBXM1)) / src,
                                        trans_[idx].FixedDensityExponentBXM1));
    }

    // If either the source or target hit totals are zero we can just write out zero:
    if (n_tgt_tot == 0. || n_src_tot == 0.) {
      trans_hits_[idx].nhits10 = 0;
      trans_hits_[idx].nhits20 = 0;
      trans_hits_[idx].nhits30 = 0;
    } else {
      double new_hits30 = std::min(n_tgt_tot, double(orig_hits_[idx].nhits30) *
                                                  (n_tgt_tot / n_src_tot) * trans_[idx].ScaleN30);
      // Override this in the FixeDensity2 mode
      if (trans_[idx].type == 5) {
        new_hits30 = n_tgt_30;
      }
      int i_n_tgt_tot = int(n_tgt_tot + 0.5);    // Round to nearest int
      int i_new_hits30 = int(new_hits30 + 0.5);  // Round to nearest int
      int i_new_hits10plus20 = i_n_tgt_tot - i_new_hits30;

      int i_new_hits20 = 0;
      if (orig_hits_[idx].nhits20 > 0) {
        i_new_hits20 = int(0.5 + (double(i_new_hits10plus20) * double(orig_hits_[idx].nhits20) /
                                  double(orig_hits_[idx].nhits10 + orig_hits_[idx].nhits20)));
      }
      int i_new_hits10 = i_new_hits10plus20 - i_new_hits20;

      // Now transform BXM1 separately
      double new_hitsBXM1 = 0.;
      // but only if n_src_tot is larger than zero
      if (n_src_tot > 0.) {
        new_hitsBXM1 = (n_tgt_tot / n_src_tot) * double(orig_hits_[idx].nhitsBXM1);
      }
      if (trans_[idx].type == 5) {
        new_hitsBXM1 = n_tgt_bxm1;
      }
      int i_new_hitsBXM1 = int(new_hitsBXM1 + 0.5);
      trans_hits_[idx].nhits10 = i_new_hits10;
      trans_hits_[idx].nhits20 = i_new_hits20;
      trans_hits_[idx].nhits30 = i_new_hits30;
      trans_hits_[idx].nhitsBXM1 = i_new_hitsBXM1;
    }
    // std::cout << mods_[idx] << "\t" << trans_[idx] << "\t" << orig_hits_[idx] << "\t" <<
    // trans_hits_[idx] << "\n";
  }
}

unsigned long HGCHitProvider::getEntries() const {
  if (t_) {
    return t_->GetEntries();
  } else {
    return 0;
  }
}

bool HGCHitProvider::nextEvent() {
  if (read_mode_ == 3 || read_mode_ == 4) {
    if (average_hits_.size() == 0) {
      average_hits_.resize(trans_hits_.size());
      packet_size_sum_.resize(trans_hits_.size());
      unsigned long entries = this->getEntries();
      // compute averages
      for (unsigned i = 0; i < entries; ++i) {
        this->processEvent(i);
        for (unsigned j = 0; j < trans_hits_.size(); ++j) {
          if (read_mode_ == 3) {
            average_hits_[j].nhits20 += trans_hits_[j].nhits20;
            average_hits_[j].nhits30 += trans_hits_[j].nhits30;
            average_hits_[j].nhits10 += trans_hits_[j].nhits10;
            average_hits_[j].nhitsBXM1 += trans_hits_[j].nhitsBXM1;
          }
          if (read_mode_ == 4) {
            packet_size_sum_[j] += ((EcondAsicHitCounts::daqRawDataPacketWords(econ_defs_[j],trans_hits_[j].nhits10,trans_hits_[j].nhits20,trans_hits_[j].nhits30)/2) - 2);
          }
        }
        if (i % 1000 == 0) {
          std::cout << "[HGCHitProvider::nextEvent] Processed " << i << "/" << entries << " events for averages" << std::endl;
        }
      }
      if (read_mode_ == 3) {
        for (unsigned j = 0; j < average_hits_.size(); ++j) {
          average_hits_[j].nhits10 = int((double(average_hits_[j].nhits10) / double(entries))+0.5);
          average_hits_[j].nhits20 = int((double(average_hits_[j].nhits20) / double(entries))+0.5);
          average_hits_[j].nhits30 = int((double(average_hits_[j].nhits30) / double(entries))+0.5);
          average_hits_[j].nhitsBXM1 = int((double(average_hits_[j].nhitsBXM1) / double(entries))+0.5);
        }
      }
      if (read_mode_ == 4) {
        for (unsigned j = 0; j < average_hits_.size(); ++j) {
          average_hits_[j].nhits10 = int((double(packet_size_sum_[j]) / double(entries))+0.5);
          average_hits_[j].nhits20 = 0xffff;
          average_hits_[j].nhits30 = 0xffff;
          average_hits_[j].nhitsBXM1 = 0;
        }
      }
      trans_hits_ = average_hits_;
    }
    return true;
  }


  if (read_mode_ == 1) {
    if (event_queue_.empty()) {
      initRandomRun();
    }
    current_evt_ = event_queue_.top();
    event_queue_.pop();
  }
  if (read_mode_ == 2 && current_evt_ == this->getEntries()) {
    // Reset back to the beginning of the file
    std::cout << "[HGCHitProvider::nextEvent] Reached final entry, cycling" << std::endl;
    current_evt_ = 0;
  }
  if (current_evt_ < this->getEntries()) {
    this->processEvent(current_evt_);
    ++current_evt_;
    return true;
  } else {
    return false;
  }
}

void HGCHitProvider::setFile(std::string const& filename, std::string const& treename) {
  // Process a single file
  // First reset the TChain
  if (t_ == nullptr) {
    t_ = new TChain();
  }
  t_->Reset();
  int result = t_->AddFile(filename.c_str(), TTree::kMaxEntries, treename.c_str());
  if (result == 0) {  // something went wrong
    throw std::runtime_error("HGCHitProvider::setFile TTree " + treename + " not found in file " + filename);
  }
  evt_.Init(t_);
  current_evt_ = 0;
}

void HGCHitProvider::setFileList(std::string const& filename, std::string const& treename) {
  // Process a list of files
  // First reset the TChain
  if (t_ == nullptr) {
    t_ = new TChain();
  }
  t_->Reset();
  auto files = ParseFileLines(filename);
  for (auto const& f : files) {
    int result = t_->AddFile(f.c_str(), TTree::kMaxEntries, treename.c_str());
    if (result == 0) {  // something went wrong
      throw std::runtime_error("HGCHitProvider::setFileList TTree " + treename + " not found in file " + f);
    }
  }
  evt_.Init(t_);
  current_evt_ = 0;
}


void HGCHitProvider::initRandomRun() {
  unsigned long N = this->getEntries();
  unsigned long nevts =
      TMath::Range(ULong_t(1), ULong_t(N - 1), ULong_t(random_frac_ * double(N) + 0.5));
  std::cout << "HGCHitProvider::initRandomRun() Generating random sequence of " << nevts << "/" << N
            << " events\n";
  for (unsigned long ir = 0; ir < nevts; ++ir) {
    double x = RandomSingleton::random().Uniform(0., double(N));
    event_queue_.push(TMath::Range(ULong_t(0), ULong_t(N - 1), ULong_t(x)));
  }
}

std::vector<int> HGCHitProvider::hits10(bool orig) const {
  std::vector<int> res(orig_hits_.size());
  if (orig) {
    for (unsigned i = 0; i < orig_hits_.size(); ++i) {
      res[i] = orig_hits_[i].nhits10;
    }
  } else {
    for (unsigned i = 0; i < trans_hits_.size(); ++i) {
      res[i] = trans_hits_[i].nhits10;
    }
  }
  return res;
}

std::vector<int> HGCHitProvider::hits20(bool orig) const {
  std::vector<int> res(orig_hits_.size());
  if (orig) {
    for (unsigned i = 0; i < orig_hits_.size(); ++i) {
      res[i] = orig_hits_[i].nhits20;
    }
  } else {
    for (unsigned i = 0; i < trans_hits_.size(); ++i) {
      res[i] = trans_hits_[i].nhits20;
    }
  }
  return res;
}

std::vector<int> HGCHitProvider::hits30(bool orig) const {
  std::vector<int> res(orig_hits_.size());
  if (orig) {
    for (unsigned i = 0; i < orig_hits_.size(); ++i) {
      res[i] = orig_hits_[i].nhits30;
    }
  } else {
    for (unsigned i = 0; i < trans_hits_.size(); ++i) {
      res[i] = trans_hits_[i].nhits30;
    }
  }
  return res;
}

std::vector<int> HGCHitProvider::hitsBXM1(bool orig) const {
  std::vector<int> res(orig_hits_.size());
  if (orig) {
    for (unsigned i = 0; i < orig_hits_.size(); ++i) {
      res[i] = orig_hits_[i].nhitsBXM1;
    }
  } else {
    for (unsigned i = 0; i < trans_hits_.size(); ++i) {
      res[i] = trans_hits_[i].nhitsBXM1;
    }
  }
  return res;
}

std::vector<std::string> HGCHitProvider::ParseFileLines(std::string const& file_name) const {
  // Build a vector of input files
  std::vector<std::string> files;
  std::ifstream file;
  file.open(file_name.c_str());
  if (!file.is_open()) {
    throw std::runtime_error(("File " + file_name + " could not be opened"));
  }
  std::string line = "";
  while (std::getline(file, line)) {  // while loop through lines
    files.push_back(line);
  }
  file.close();
  return files;
}
