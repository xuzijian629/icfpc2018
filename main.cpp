#include <bits/stdc++.h>
using namespace std;
using VI = vector<int>;
using VVI = vector<VI>;
using VVVI = vector<VVI>;

int R;
VVVI model;

void read_binary() {
  cout << "Model file name?" << endl;
  string s;
  cin >> s;
  FILE *fp;
  if ((fp = fopen(s.c_str(), "rb")) == NULL) {
    cerr << "Failed to open file" << endl;
    exit(1);
  }

  uint8_t r;
  fread(&r, sizeof(uint8_t), 1, fp);
  R = int(r);
  model = VVVI(R, VVI(R, VI(R)));
  string bin = "";
  char c[2000000];
  assert(ceil(R * R * R / 8.0) < 2000000);
  fread(c, sizeof(char), int(ceil(R * R * R / 8.0)), fp);
  fclose(fp);
  for (int i = 0; i < int(ceil(R * R * R / 8.0)); i++) {
    bin += '0' + ((c[i] >> 3) & 1);
    bin += '0' + ((c[i] >> 2) & 1);
    bin += '0' + ((c[i] >> 1) & 1);
    bin += '0' + (c[i] & 1);
  }
  for (int i = 0; i < R; i++) {
    for (int j = 0; j < R; j++) {
      for (int k = 0; k < R; k++) {
        model[i][j][k] = bin[i * R * R + j * R + k] == '1';
      }
    }
  }
  cout << "Model successfully imported" << endl;
}

int main() {
  read_binary();

}
