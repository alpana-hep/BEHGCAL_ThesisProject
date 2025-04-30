#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "Arguments.h"

int main(int argc, char *argv[]) {
  Arguments args;
  args.processArguments(argc,argv);
  args.print();
  
  std::ofstream slpgbt("test/single.txt", std::ios::out);
  std::ofstream dlpgbt("test/double.txt", std::ios::out);

  // Mapping file can be changed using -m <filename>
  std::ifstream mapfile(std::string("data/FE/")+args.mappingFile(), std::ios::in);

  //variables to count different cases..
  //char c = 'c';
  //int nTrue_Module = 0;  // for modules that have less than or equal to 14 e-links
  //int nFalse_Module = 0; // to count modules having unphysical number of e-links(>14)
  //int nM_zero_Lpgbt =0; // to count how many modules are there having zero lpgbts connected to them
  //int nM_HD =0, nM_LD=0, nM_ST=0; // HD= high density, LD = low density, ST = scintillator tiles
  //int nM_zero_shape_not_c =0; // count any exceptions having 0 lpgbts but not shape c
  //int Total_eLinks[1680]={};
  //int Total_eLinks_Module[5550] ={};
  //int Lpgbt_Numbers[1680]={};
  //int count =0, count0=0;
  //std::string line;
  struct lpGBT_Definition_Map {
    unsigned int  layer, u, v, type;
    char shape;
    unsigned int  nDaqLpgbt;
    int idxDaqLpgbt1, idxDaqLpgbt2;
    int  nDaqElinks1, nDaqElinks2;
    //    int  nTpgLpgbt, idxTpgLpgbt1, nTpgElinks1, idxTpgLpgbt2, nTpgElinks2;
    std::pair< int, int> idxLpgbt;
  };
  
  std::vector<lpGBT_Definition_Map> lpgbt_vector;

  struct modulemap {
    unsigned int  layer, u, v, type;
    char shape;
    int  nDaqElinks1, nDaqElinks2;
    unsigned int  nDaqLpgbt,nTpgLpgbt;
    int  idxDaqLpgbt1, idxDaqLpgbt2;
    unsigned int  idxTpgLpgbt1, nTpgElinks1, idxTpgLpgbt2, nTpgElinks2;
  };
  std::vector <modulemap> map_vec;

  if(!mapfile.is_open()) {
    std::cout << " file not opened" << std::endl;
  } else {
    map_vec.clear();
    while (true) {
      unsigned int  layer, u, v, type;
      char shape;
      int   idxDaqLpgbt1, idxDaqLpgbt2;
      int  nDaqElinks1, nDaqElinks2;
      int idxTpgLpgbt1, nTpgElinks1, idxTpgLpgbt2, nTpgElinks2;
      unsigned nTpgLpgbt, nDaqLpgbt;
       
      mapfile >> layer >> u >> v >> type >> shape 
	      >> nDaqLpgbt >> nTpgLpgbt 
	      >> idxDaqLpgbt1 >> nDaqElinks1 >> idxDaqLpgbt2 >> nDaqElinks2
	      >> idxTpgLpgbt1 >> nTpgElinks1 >> idxTpgLpgbt2 >> nTpgElinks2;
       if (mapfile.eof()) break;
      modulemap mm;
      mm.layer=layer; mm.u=u; mm.v=v; mm.type=type;
      mm.shape=shape;
      mm.nDaqLpgbt=nDaqLpgbt; mm.idxDaqLpgbt1=idxDaqLpgbt1; mm.nDaqElinks1=nDaqElinks1;
      mm.idxDaqLpgbt2=idxDaqLpgbt2; mm.nDaqElinks2= nDaqElinks2;
      mm.nTpgLpgbt=nTpgLpgbt; mm.idxTpgLpgbt1=idxTpgLpgbt1; mm.nTpgElinks1=nTpgElinks1;
      mm.idxTpgLpgbt2=idxTpgLpgbt2; mm.nTpgElinks2= nTpgElinks2;

      map_vec.push_back(mm);
      /*
      std::cout << layer <<" "<< u <<" "<< v <<" "<< type <<" "<< shape 
		<<"\t"<< nDaqLpgbt <<" "<< nTpgLpgbt
		<<"\t"<< idxDaqLpgbt1 <<" "<< nDaqElinks1 <<"\t"<< idxDaqLpgbt2 <<" "<< nDaqElinks2
		<<"\t"<< idxTpgLpgbt1 <<" "<< nTpgElinks1 <<"\t"<< idxTpgLpgbt2 <<" "<< nTpgElinks2
		<< std::endl;
      */
      // if (mapfile.eof()) break;
    }
    mapfile.close();
  }
  int count =0, count0 =0;
  std::cout << map_vec.size() << std::endl;

  std::cout<<map_vec[0].u<<"\t"<<map_vec[0].v<< std::endl;  
  //  std::map<unsigned int,std::pair<unsigned int,unsigned int>> newmap;
  //std::vector <newmap> pair_vec;

  //  std::pair<unsigned int,unsigned int> idxLpgbt(map_vec[imap].idxDaqLpgbt1, map_vec[imap].idxDaqLpgbt2);
  //std::pair<std::pair<unsigned int,unsigned int>, modulemap> prePair(idxLpgbt,  map_vec[imap]);

  for(int imap=0; imap<5508; imap++)
    {
      //    std::cout<<mymap[imap].u<<"\t"<<mymap[imap].v<< std::endl;                                                                 
      //      std::cout<<map_vec[imap].u<<"\t"<<map_vec[imap].v<< std::endl;
      lpGBT_Definition_Map map;
      //	 map.idxLpgbt(map_vec[imap].idxDaqLpgbt1, map_vec[imap].idxDaqLpgbt2); 
      //  std::pair<std::pair<unsigned int,unsigned int>, modulemap> prePair(idxLpgbt,  map_vec[imap]);
      //std::vector <prePair<std::pair<unsigned int,unsigned int> pair_vec
      int j = map_vec[imap].idxDaqLpgbt1;
      int k = map_vec[imap].idxDaqLpgbt2;
      
      //      modulemap 
      if ( map_vec[imap].nDaqLpgbt !=0)
	{
	 
	  //for double lpGBTs
	  if(k!=-1)
	    {
	      count++;
	      //std::cout<<j<<"\t"<<k<< std::endl;
	      map.layer = map_vec[imap].layer;
	      map.u = map_vec[imap].u;
	      map.v = map_vec[imap].v;
	      map.type = map_vec[imap].type;
	      map.shape = map_vec[imap].shape;
	      map.nDaqLpgbt = 2;
	      map.idxDaqLpgbt1 = map_vec[imap].idxDaqLpgbt1;
	      map.idxDaqLpgbt2 = map_vec[imap].idxDaqLpgbt2;
	      map.nDaqElinks1 = map_vec[imap].nDaqElinks1;
	      map.nDaqElinks2 =map_vec[imap].nDaqElinks2;
	      map.idxLpgbt.first= j;
	      map.idxLpgbt.second =k;
	      lpgbt_vector.push_back(map);
	
	      //	    std::cout<<imap<< std::endl;
	      //	    std::cout<<map_vec[imap].u<<"\t"<<map_vec[imap].v<< std::endl;
	   
	      for(int l = imap-2;l<imap ;l++)
		{
		  // std::cout<<l<<std::endl;
		  if(l<0)
		    {continue;}
		  else  if (j==  map_vec[l].idxDaqLpgbt1 && map_vec[l].nDaqLpgbt==1 && map_vec[l].idxDaqLpgbt2 ==-1)
		    {
		      count++;
		      //std::cout<<j<<"\t"<<k<< std::endl;                                                                
		      map.layer = map_vec[l].layer;
		      map.u = map_vec[l].u;
		      map.v = map_vec[l].v;
		      map.type = map_vec[l].type;
		      map.shape = map_vec[l].shape;
		      map.nDaqLpgbt = 2;
		      map.idxDaqLpgbt1 = map_vec[l].idxDaqLpgbt1;
		      map.idxDaqLpgbt2 = map_vec[l].idxDaqLpgbt2;
		      map.nDaqElinks1 = map_vec[l].nDaqElinks1;
		      map.nDaqElinks2 =map_vec[l].nDaqElinks2;
		      map.idxLpgbt.first= j;
		      map.idxLpgbt.second =k;
		      lpgbt_vector.push_back(map);
		      //std::cout<<l<<std::endl;
		      //       std::cout<<map_vec[l].u<<"\t"<<map_vec[l].v<< std::endl;
		      //		    std::cout<<l<<std::endl;
		    }
		}
	      for(int l = imap+1;l<imap+3;l++)
		{
		  if (k== map_vec[l].idxDaqLpgbt1 &&  map_vec[l].nDaqLpgbt==1)
		    {
		      count++;
		      //std::cout<<j<<"\t"<<k<< std::endl;
		      map.layer = map_vec[l].layer;
                      map.u = map_vec[l].u;
                      map.v = map_vec[l].v;
                      map.type = map_vec[l].type;
                      map.shape = map_vec[l].shape;
		      map.nDaqLpgbt = 2;
                      map.idxDaqLpgbt1 = map_vec[l].idxDaqLpgbt1;
                      map.idxDaqLpgbt2 = map_vec[l].idxDaqLpgbt2;
                      map.nDaqElinks1 = map_vec[l].nDaqElinks1;
                      map.nDaqElinks2 =map_vec[l].nDaqElinks2;
		      map.idxLpgbt.first= j;
		      map.idxLpgbt.second =k;
		      lpgbt_vector.push_back(map);

		      // std::cout<<map_vec[l].u<<"\t"<<map_vec[l].v<< std::endl;
		      // std::cout<<l<<std::endl;
		    }
		}
	      //std::cout<<count<<std::endl;
	      //std::cout<<j<<"\t"<<k<< std::endl;
	      // map.nDaqLpgbt = count;
	      //lpgbt_vector.push_back(map);
	      //	      count =0;
	    }
	  //for single lpGBTs
	  
	  //else if((k==-1 && j != map_vec[imap-1].idxDaqLpgbt2 ) && (j!=map_vec[imap-2].idxDaqLpgbt2) && (map_vec[imap+1].nDaqLpgbt !=2))
	  //{
	  else 
	    {
	      //	      std::cout<<map_vec[imap].u<<"\t"<<map_vec[imap].v<< std::endl;
	      for(int l = imap-1;l<imap;l++)
		{ 
		  //std::cout<<map_vec[l].u<<"\t"<<map_vec[l].v<< std::endl;
		  if(l<0)
		    { continue;}
		  else if(k==-1 && j != map_vec[l].idxDaqLpgbt2 && j != map_vec[l-1].idxDaqLpgbt2 && map_vec[l+2].nDaqLpgbt !=2)
		  //		  else if (k==-1)
		    {
		      count++;
		      // map.layer = map_vec[l].layer;
                      // map.u = map_vec[l].u;
                      // map.v = map_vec[l].v;
                      // map.type = map_vec[l].type;
                      // map.shape = map_vec[l].shape;
		      // map.nDaqLpgbt = 1;
                      // map.idxDaqLpgbt1 = map_vec[l].idxDaqLpgbt1;
                      // map.idxDaqLpgbt2 = map_vec[l].idxDaqLpgbt2;
                      // map.nDaqElinks1 = map_vec[l].nDaqElinks1;
                      // map.nDaqElinks2 =map_vec[l].nDaqElinks2;

		      // map.idxLpgbt.first= j;
		      // map.idxLpgbt.second =k;
		      // lpgbt_vector.push_back(map);
		      
		      map.layer = map_vec[imap].layer;
                      map.u = map_vec[imap].u;
                      map.v = map_vec[imap].v;
                      map.type = map_vec[imap].type;
                      map.shape = map_vec[imap].shape;
                      map.nDaqLpgbt = 1;
                      map.idxDaqLpgbt1 = map_vec[imap].idxDaqLpgbt1;
                      map.idxDaqLpgbt2 = map_vec[imap].idxDaqLpgbt2;
                      map.nDaqElinks1 = map_vec[imap].nDaqElinks1;
                      map.nDaqElinks2 =map_vec[imap].nDaqElinks2;

                      map.idxLpgbt.first= j;
                      map.idxLpgbt.second =k;
                      lpgbt_vector.push_back(map);

		       std::cout<<map_vec[imap].layer<<"\t"<<map_vec[imap].u<<"\t"<<map_vec[imap].v<< std::endl;
		      //std::cout<<j<<"\t"<<k<<std::endl;
		    }

		}
	    }
	
	  //	  map.nDaqLpgbt =count0;
	  //lpgbt_vector.push_back(map);
	  //count0 =0;
	}
     
 
    }
  std::cout<<lpgbt_vector.size()<<std::endl;
  std::cout<<count<<std::endl;
  //read the vector
  for( int ilpgbt =0; ilpgbt<5324;ilpgbt++)
    {
      
      //      std::cout<<lpgbt_vector[ilpgbt].idxLpgbt.first<<"\t"<<lpgbt_vector[ilpgbt].idxLpgbt.second<<std::endl;
      if((lpgbt_vector[ilpgbt].idxLpgbt.second ==-1) && (lpgbt_vector[ilpgbt].idxLpgbt.first == lpgbt_vector[ilpgbt+1].idxLpgbt.first && lpgbt_vector[ilpgbt+1].idxLpgbt.second != -1))
	{
	  
	  lpgbt_vector.erase(lpgbt_vector.begin()+ilpgbt);

	}
      else
	{ count0++;	
	  continue;}

	
    }
  std::cout<<count0<<std::endl;
  std::cout<<lpgbt_vector.size()<<std::endl;
  //std::setw (unsigned int n);
  for( int i =0; i<5300; i++)
    {
      // std::cout<<lpgbt_vector[i].idxLpgbt.first<<"\t"<<lpgbt_vector[i].idxLpgbt.second<<std::endl;
      if(lpgbt_vector[i].idxLpgbt.second == -1)
	{

	  if(!slpgbt)
	    {
	      std::cout<<",,,,,,,,,,,"<<std::endl;
	    }
	  else

	    {
	      unsigned int  layer, u, v, type;
	      char shape;
	      unsigned nDaqLpgbt;
	      int  idxDaqLpgbt1, idxDaqLpgbt2;
	      int nDaqElinks1, nDaqElinks2;
	      layer= lpgbt_vector[i].layer;
	      u = lpgbt_vector[i].u;
	      v = lpgbt_vector[i].v;
	      type = lpgbt_vector[i].type;
	      shape = lpgbt_vector[i].shape;
	      nDaqLpgbt = lpgbt_vector[i].nDaqLpgbt;
	      idxDaqLpgbt1 = lpgbt_vector[i].idxLpgbt.first;
	      idxDaqLpgbt2 = lpgbt_vector[i].idxLpgbt.second;
	      nDaqElinks1 = lpgbt_vector[i].nDaqElinks1;
	      nDaqElinks2 = lpgbt_vector[i].nDaqElinks2;
	      
	      // fill in the txt file
	      slpgbt << layer <<"\t"<< u << "\t" << v << "\t"<< type << "\t"<< shape << "\t"<< nDaqLpgbt << "\t"<< idxDaqLpgbt1 << "\t"<< nDaqElinks1  << "\t"<< idxDaqLpgbt2 << "\t"<<  nDaqElinks2 << "\n";
	     

	    }// while loop
	}
      else
	{
	  if(!dlpgbt)
            {
	      std::cout<<",,,,,,,,,,,"<<std::endl;
            }
          else

            {
              unsigned int  layer, u, v, type;
              char shape;
	      unsigned nDaqLpgbt;
	      int   idxDaqLpgbt1, idxDaqLpgbt2;
	      int nDaqElinks1,nDaqElinks2;
              layer= lpgbt_vector[i].layer;
              u = lpgbt_vector[i].u;
              v = lpgbt_vector[i].v;
              type = lpgbt_vector[i].type;
              shape = lpgbt_vector[i].shape;
              nDaqLpgbt = lpgbt_vector[i].nDaqLpgbt;
              idxDaqLpgbt1 = lpgbt_vector[i].idxLpgbt.first;
              idxDaqLpgbt2 = lpgbt_vector[i].idxLpgbt.second;
              nDaqElinks1 = lpgbt_vector[i].nDaqElinks1;
              nDaqElinks2 = lpgbt_vector[i].nDaqElinks2;
	      // fill the txt file
	      dlpgbt << layer <<"\t"<< u << "\t" << v << "\t"<< type << "\t"<< shape << "\t"<< nDaqLpgbt << "\t"<< idxDaqLpgbt1 << "\t"<< nDaqElinks1  << "\t"<< idxDaqLpgbt2 << "\t"<<  nDaqElinks2 << "\n";
	    }
	}
    }
    
  slpgbt.close();
  dlpgbt.close();
  return 0;

    };
