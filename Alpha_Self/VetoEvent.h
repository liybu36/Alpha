#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <map>
#include <ctime>

struct VetoEvent{
VetoEvent(): od_eventID(-1), od_nclusters(-1), od_gps_fine(-1),
    od_gps_coarse(-1), od_timestamp(-1), od_wt_charge(-1)
    //,od_counts(0)                                                                                            
  {}
  //  Int_t od_counts;                                                                                                              
  Int_t od_eventID;
  Int_t od_nclusters;
  Double_t od_gps_fine;
  Double_t od_gps_coarse;
  Double_t od_timestamp; //[us]                                                                                                     
  Double_t od_tpc_timestamp_diff; //[us] timestamp difference between tpc and veto. od_timestamp - tpc_timestamp                    
  Double_t od_wt_charge;
  std::vector<Double_t> od_cluster_charge; //[PE]                                                                                  
  std::vector<Double_t> od_cluster_start; //[us]                                                                                   
  std::vector<Double_t> od_cluster_height;
  std::vector<Double_t> od_cluster_multiplicity;
  std::vector<Int_t> od_cluster_pass_multcut;
  std::vector<Double_t> od_cluster_dtprompt; //[us]                                                                                 
};
