#include <iostream>
#include <fstream>

using namespace std;

class cccut{
  ifstream src;
  struct{
    char str[64];
    unsigned int num[2];
  } cc_header;
  struct{
    char filename[128];
    unsigned int offset[2];
    unsigned int size[2];
  } cc_index;

 public:
  cccut(){}
  ~cccut(){}

  int open(char *name);
  void close(){ src.close(); }
  void output(unsigned int ident);
  int num(){ return cc_header.num[0]; }
};

int cccut::open(char *name)
{
  src.open( name );
  if( !src ){
    perror( name );
    return -1;
  }
  src.read( cc_header.str, sizeof(cc_header) );
  cout << "Recognizing " << cc_header.str << "..." << endl;
  cout << cc_header.num[0] << " files." << endl;
  return 0;
}

void cccut::output(unsigned int n)
{
  unsigned char data;
  ofstream stream;

  src.seekg( sizeof(cc_header)+sizeof(cc_index)*n );
  src.read( cc_index.filename, sizeof(cc_index) );
  if( cc_index.filename[0] == '\0' ) return;

  cout << "Unpacking " << cc_index.filename << " at " << hex << cc_index.offset[0] << ":" << cc_index.size[0] << "...";
  cout.flush();

  src.seekg( cc_index.offset[0] );
  stream.open( cc_index.filename );
  if( !stream ){
    perror( cc_index.filename );
    exit( 1 );
  }
  for(unsigned int i=0; i<cc_index.size[0]; i++ ){
    src.read( (std::basic_istream<char>::char_type*)&data, 1 );
    stream.write( (const char*)&data, 1 );
  }
  stream.close();
  cout << "done." << endl;
}

int cc_usage()
{
  cerr << "Usage: cccut <.pd archive file> [packed file no.]" << endl;
  return 0;
}

int main(int argc, char *argv[])
{
  cccut cc;

  if( argc == 1 ) return cc_usage();

  if( cc.open( argv[1] ) < 0 ) return 1;
  if( argc > 2 ){
    if( atoi(argv[2]) < cc.num() )
      cc.output( atoi(argv[2]) );
    else
      cerr << "invalid file no." << endl;
  } else
    for(int i=0; i<cc.num(); i++ ) cc.output( i );
  cc.close();
  return 0;
}
