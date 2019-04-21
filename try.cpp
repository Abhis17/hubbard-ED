bool diagonalize(MatrixXd Ac, VectorXd& lambdac, MatrixXd& vc)
{
  int N;

  if(Ac.cols()==Ac.rows())  N = Ac.cols(); else return false;
  

  lambdac.resize(N);
  vc.resize(N,N);
  
  int LDA = N;
  int INFO = 0;
  char Uchar = 'U';
  char Vchar = 'V';
  char Nchar = 'N';

  int LWORK = 5*(2*LDA*LDA+6*LDA+1);
  int LIWORK = 5*(3+5*LDA);
  cout << LWORK << " " << LIWORK << endl;

  VectorXd WORK(LWORK);
  VectorXi IWORK(IWORK);

  dsyevd_(&Vchar, &Uchar, &N, Ac.data(), &LDA, lambdac.data(),  WORK.data(), &LWORK, IWORK.data(), &LIWORK, &INFO);
  
  vc = Ac;
  return INFO==0;
}

bool diagonalize(MatrixXd Ac, std::vector<double>& v, MatrixXd& vc)
{
  VectorXd lambdac;
  bool result = diagonalize(Ac,lambdac,vc);
  v.resize(lambdac.size());
  VectorXd::Map(&v[0], lambdac.size()) = lambdac;
  return result;
}