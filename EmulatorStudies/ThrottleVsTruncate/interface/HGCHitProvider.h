#ifndef Emulation_HGCalBufferModel_HGCHitProvider
#define Emulation_HGCalBufferModel_HGCHitProvider

#include <string>
#include <iostream>
#include <queue>

#include "TChain.h"
#include "EventTree.h"
#include "EcondAsicDefinition.h"
#include "RandomSingleton.h"

class HGCHitProvider {
 public:
  HGCHitProvider();
  ~HGCHitProvider();

  void initialise(std::vector<EcondAsicDefinition> const& defs, std::string const& config);

  void initialise(std::string const& mapping, std::string const& config);

  void setFile(std::string const& filename, std::string const& treename);

  void setFileList(std::string const& filelist, std::string const& treename);

  inline void setReadUnmasked(bool readUnmasked) {read_unmasked_ = readUnmasked;}

  inline void setReadMode(int readMode) { read_mode_ = readMode; }

  inline void setRandomFrac(double randomFrac) { random_frac_ = randomFrac; }

  inline void setCurrentEvent(unsigned long evt) { current_evt_ = evt; }

  unsigned long getEntries() const;

  bool nextEvent();

  void processEvent(unsigned long ievt);

  inline EventTree const& eventTree() const {
    return evt_;
  }

  struct Module {
    int layer;
    int type;
    int u;
    int v;
    char s;
    bool hdm = false;

    bool operator<(const Module& m2) const {
      return this->layer < m2.layer ||
             (this->layer == m2.layer && (this->u < m2.u || (this->u == m2.u && this->v < m2.v)));
    }

    bool operator==(const Module& m2) const {
    	return this->layer == m2.layer && this->u == m2.u && this->v == m2.v;
    }

    friend std::ostream &operator<<(std::ostream &output, Module const& mod) {
       output << "[" << mod.layer << "," << mod.type << "," << mod.u << "," << mod.v << "]";
       return output;
    }
  };

  struct Hits {
    int nhits10 = 0;
    int nhits20 = 0;
    int nhits30 = 0;
    int nhitsBXM1 = 0;

    friend std::ostream& operator<<(std::ostream& output, Hits const& h) {
      output << "[" << h.nhits10 << "," << h.nhits20 << ", " << h.nhits30 << "]";
      return output;
    }
  };

  struct TransformMethod {
    int type = 0;
    int FixedNumberMax = 0;
    double FixedOccupancyFrac = 0.;
    double FixedDensitySrc = 0.;
    double FixedDensityTarget = 0.;
    double ScaleN30 = 1.0;
    double FixedDensityExponentTotal = 1.0;
    double FixedDensityExponentTOA = 1.0;
    double FixedDensityExponentBXM1 = 1.0;
    int nCells = 0;

    friend std::ostream& operator<<(std::ostream& output, TransformMethod const& trans) {
      output << "[";
      if (trans.type == 0) {
        output << "Zero";
      } else if (trans.type == 1) {
        output << "NoTransform";
      } else if (trans.type == 2) {
        output << "FixedNumber[" << trans.FixedNumberMax << "]";
      } else if (trans.type == 3) {
        output << "FixedOccupancy[" << trans.FixedOccupancyFrac << "]";
      } else if (trans.type == 4) {
        output << "FixedDensity[" << trans.FixedDensitySrc << ", " << trans.FixedDensityTarget << "]";
      } else if (trans.type == 5) {
        output << "FixedDensity2[" << trans.FixedDensitySrc << ", " << trans.FixedDensityTarget << ", " << trans.FixedDensityExponentTotal << ", " << trans.FixedDensityExponentTOA << ", " << trans.FixedDensityExponentBXM1 << "]";
      }
      output << ", ScaleN30[" << trans.ScaleN30 << "]]";
      return output;
    }
  };

  inline std::vector<Module> const& modules() const {
  	return mods_;
  }

  inline std::vector<Hits> const& origHits() const {
  	return orig_hits_;
  }

  inline std::vector<Hits> const& transformedHits() const {
  	return trans_hits_;
  }

  std::vector<int> hits10(bool orig) const;
  std::vector<int> hits20(bool orig) const;
  std::vector<int> hits30(bool orig) const;
  std::vector<int> hitsBXM1(bool orig) const;

 private:
  bool read_unmasked_;
  std::vector<Module> mods_;
  std::vector<EcondAsicDefinition> econ_defs_;
  std::vector<unsigned> packet_size_sum_;
  std::vector<Hits> orig_hits_;
  std::vector<Hits> trans_hits_;
  std::vector<Hits> average_hits_;
  std::vector<TransformMethod> trans_;

  TChain* t_ = nullptr;
  EventTree evt_;
  unsigned long current_evt_;

  int read_mode_ = 0;  // 0 = sequential  // 1 = random
  double random_frac_ = 0.01; // fraction of events to read in each pass
  std::priority_queue<unsigned long, std::vector<unsigned long>, std::greater<unsigned long>> event_queue_;

  void initRandomRun();

  std::vector<std::string> ParseFileLines(std::string const& file_name) const;
};

#endif
