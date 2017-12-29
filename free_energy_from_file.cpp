#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <Eigen/Dense>
// #include "edlib.h"
// #include "common.h"
int size;
float t=1;
float U;
float epsilon= 1e-6;
using namespace std;
using namespace Eigen;

float find_free_energy(float temperature, vector<float> eigenvalues)
{
  float partition_func = 0;
  std::sort (eigenvalues.begin(), eigenvalues.end());
  float unruly_free_energy= 0;

  if(isinf(exp(-eigenvalues.at(0)/temperature)))
  {
    unruly_free_energy += eigenvalues.at(0);
    transform(eigenvalues.begin(), eigenvalues.end(), eigenvalues.begin(), bind1st(plus<float>(),-eigenvalues.at(0)));
  }
  for(auto it=eigenvalues.begin(); it!=eigenvalues.end(); it++)
  {
    partition_func += exp(-(*it)/temperature);
  }
  cout << unruly_free_energy << " \t" << partition_func << endl;
  float free_energy = unruly_free_energy + (-temperature)*log(partition_func);
  return free_energy;
}

float get_mu(float temperature, std::vector<float> v)
{
  sort (v.begin(), v.end());
  float bisection_up_lim = v.back();
  float bisection_low_lim = v.front();

  float mu;
  float no_of_electrons; int count=0;

  for(; ;)
  {
    int it=0; no_of_electrons=0;  count++;
    cout << "Loop:" << count << "\n----------------------\n";
    mu = 0.5*(bisection_low_lim+bisection_up_lim) ;

    while(no_of_electrons< float(size))
    {
      float fermi_func = 1/(exp((v[it]-mu)/temperature)+1);
      // std::cout << fermi_func << '\t';
      no_of_electrons += fermi_func;  it++;
    }
    if(abs(no_of_electrons-size) < epsilon){cout << "exact " << v[it] << ", mu=" << mu << " position= "<< it << endl;  cout << no_of_electrons << endl; return mu; break;}
    else if(no_of_electrons > size+epsilon) { cout << "upper " << mu << " " << it-1 << " " <<  no_of_electrons << endl; bisection_up_lim=mu;}
    else if(no_of_electrons < size-epsilon) { cout << "lower " << mu << " " << it-1 << " " << no_of_electrons << endl; bisection_low_lim=mu;}
    cout << "\n-----------------------------\n";
  }

}

// float maxval(std::vector<float> v) {sort(v.begin(),v.end()); return v.back();}
// float minval(std::vector<float> v) {sort(v.begin(),v.end()); return v.front();}
//
// float get_mu_s(float fill, std::vector<float> evl_s, float T)
// {
//   float f0, f, fl2, fr, mr, ml, rtmp, m_d;
//
//   mr = maxval(evl_s);
//   fr=0.0;
//   for(int i=0; i<evl_s.size(); i++)
//     fr += (1.0/(exp((evl_s[i]-mr)/T)+1.0));
//
//   ml= minval(evl_s);
//   fl2=0.0;
//   for(int i=0; i<evl_s.size(); i++)
//     fl2 += (1.0/(exp((evl_s[i]-ml)/T)+1.0));
//
//   m_d= 0.5*(ml+mr);
//   f=0.0;
//   for(int i=0; i<evl_s.size(); i++)
//     f += (1.0/(exp((evl_s[i]-m_d)/T)+1.0));
//
//     std::cerr << "===============" << endl;
//     cerr << " mr= " << mr  << " ml= " << ml << " m_d= " << m_d << " \n";
//     std::cerr << "================" << '\n';
//
//   int count =0;
//
//   while(abs(f-fill)>1e-2)
//   {
//     cerr << ++count << ") ";
//
//     m_d=0.5*(ml+mr);
//
//     cerr << " mr= " << mr  << " ml= " << ml << " m_d= " << m_d << " ";
//
//     f=0.0;
//     for(int i=0; i<evl_s.size(); i++)
//       f += (1.0/(exp((evl_s[i]-m_d)/T)+1.0));
//     if(f>fill)
//     {
//       mr= m_d; fr=f;
//       cerr << " /upper/ ";
//     }
//     else
//     {
//       ml=m_d; fr = f;
//       cerr << " /lower/ ";
//     }
//
//     cerr << " f= " << f << endl;
//     if(count==100) break;
//   }
//
//   return m_d;
// }

int main(int argc, char* argv[])
{
  if(argc!=3) exit(1);

  cout << "Enter size and U (as per input file): ";
  cin >> size >> U;

  std::vector<float> eigenvalues;
  ifstream fin(argv[1]);
    while (true)
    {
      float eival;
      fin >> eival;
      if( fin.eof() ) break;
      eigenvalues.push_back(eival);
    }

  float TEMP; cin >> TEMP;
  get_mu(TEMP,eigenvalues);
  exit(1);

  float initial_temp, final_temp, temperature_step;
  cout << "Enter initial temperature, final temperature, temperature_step: ";
  cin >> initial_temp >> final_temp >> temperature_step;

  ofstream outfilefreeenergy(argv[2]);
  for(float temperature = initial_temp; temperature < final_temp; temperature+= temperature_step)
  {
     outfilefreeenergy << temperature << " " << find_free_energy(temperature, eigenvalues) << endl;
     cout << find_free_energy(temperature,eigenvalues) << endl;
  }
}
