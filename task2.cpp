#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <map>
#include <set>

using namespace std;
using namespace std::chrono;

int getRand(int min, int max) {
    int randNum = rand()%(max - min + 1) + min;
    return randNum;
}

int brute(string T, string P) {
  int tLen = T.length();
  int pLen = P.length();

  if (pLen > tLen) {
    return -1;
  }

  int i = 0;
  int j = 0;
  while(i < tLen) {
    if (T[i] == P[j]) {
      int start = i;
      while(start < tLen && j < pLen && T[start] == P[j]) {
        start++;
        j++;
      }
      if (j == pLen) {
        return i;
      }
      j = 0;
      i++;
    } else {
      j = 0;
      i++;
    }
  }

  return -1;
}

int kmp(string T, string P) {
  int tLen = T.length();
  int pLen = P.length();

  if (pLen > tLen) {
    return -1;
  }

  vector<int> lps(pLen, 0);
  int index = 0;
  int i = 1;
  while (i < pLen) {
    if (P[i] == P[index]) {
      lps[i] = index + 1;
      index++;
      i++;
    } else {
      if (index == 0) {
        lps[i] = 0;
        i++;
      } else {
        index = lps[index - 1];
      }
    }
  }

  i = 0;
  int j = 0;

  while(i < tLen) {
    if (T[i] == P[j]) {
      int start = i;
      while(start < tLen && j < pLen && T[start] == P[j]) {
        start++;
        j++;
      }
      if (j == pLen) {
        return start - pLen;
      }
      j = lps[j - 1];
      i = start;
    } else {
      if (j == 0) {
        i++;
      } else {
        j = lps[j - 1];
      }
    }
  }

  return -1;
}

int bmh(string T, string P) {
  int tLen = T.length();
  int pLen = P.length();

  if (pLen > tLen) {
    return -1;
  }

  map<char, int> mp;
  for (int i = 0; i < pLen - 1; i++) {
    mp[P[i]] = pLen - i - 1;
  }

  if (mp.find(P[pLen - 1]) == mp.end()) {
    mp[P[pLen - 1]] = pLen;
  }

  int i = pLen - 1;
  int j = pLen - 1;

  while(i < tLen) {
    if (T[i] == P[j]) {
      int start = i;
      while(i >= 0 && j >= 0 && T[i] == P[j]) {
        i--;
        j--;
      }
      if (j < 0) {
        return i + 1;
      }
      j = pLen - 1;
      i = start + (mp.find(T[start]) == mp.end() ? pLen : mp[T[start]]);
    } else {
      i += mp.find(T[i]) == mp.end() ? pLen : mp[T[i]];
    }
  }

  return -1;
}

bool isEqual(string T, string P, int ind) {
	for (int i = 0; i < P.length(); i++) {
		if (T[ind] == P[i]) {
			ind++;
		} else {
			return false;
		}
	}

	return true;
}

int rk(string T, string P) {
	int tLen = T.length();
	int pLen = P.length();

	if (tLen == 0 && pLen == 0) {
		return 0;
	}

	if (pLen == 0) {
		return tLen;
	}

	if (pLen > tLen) {
		return -1;
	}

	int prime = 3;

	long long int pHash = 0;
	long long int tHash = 0;

	for (int i = 0; i < pLen; i++) {
		pHash = pHash + int(P[i]) * pow(prime, i);
		tHash = tHash + int(T[i]) * pow(prime, i);
	}

	for (int i = pLen; i < tLen; i++) {
		if (pHash == tHash) {
			if (isEqual(T, P, i - pLen)) {
				return i - pLen;
			} else {
				tHash = tHash - int(T[i - pLen]);
				tHash = tHash / prime;
				tHash = tHash + int(T[i]) * pow(prime, pLen - 1);
			}
		} else {
			tHash = tHash - int(T[i - pLen]);
			tHash = tHash / prime;
			tHash = tHash + int(T[i]) * pow(prime, pLen - 1);
		}
	}

    if (pHash == tHash) {
        if (isEqual(T, P, tLen - pLen)) {
            return tLen - pLen;
        }
    }

	return -1;
}

string getOrStr(string s1, string s2) {
    string res = "";
    for (int i = 0; i < s1.length(); i++) {
        if (s1[i] == '1' || s2[i] == '1') {
            res += "1";
        } else {
            res += "0";
        }
    }

    return res;
}

bool isCharEqual(map<char, string>& mp, string& state, string T, string ones, int tPos, int pPos) {
    string bitStr = "";
    if (mp.find(T[tPos]) == mp.end()) {
        bitStr = ones;
    } else {
        bitStr = mp[T[tPos]];
    }

    state = state.substr(1);
    state += "0";
    string orStr = getOrStr(state, bitStr);

    state = orStr;

    return orStr[orStr.length() - pPos - 1] == '0';
}

int bitap(string T, string P) {
    int tLen = T.length();
    int pLen = P.length();

    if (pLen > tLen) {
        return -1;
    }

    map<char, string> mp;
    set<char> st;

    string ones = "";

    for (int i = 0; i < pLen; i++) {
        st.insert(P[i]);
        ones += "1";
    }

    for (auto it : st) {
        string bitStr = "";
        for (int i = pLen - 1; i >= 0; i--) {
            if (P[i] == it) {
                bitStr += "0";
            } else {
                bitStr += "1";
            }
        }

        mp[it] = bitStr;
    }

    string state = ones;
    int pPos = 0;

    for (int i = 0; i < tLen; i++) {
        if (isCharEqual(mp, state, T, ones, i, pPos)) {
            int start = i;
            start++;
            pPos++;
            while(pPos < pLen) {
                if (isCharEqual(mp, state, T, ones, start, pPos)) {
                    start++;
                    pPos++;
                } else {
                    pPos = 0;
                    state = ones;
                    break;
                }
            }
            if (pPos == pLen) {
                return start - pLen;
            }
        } else {
            pPos = 0;
            state = ones;
        }
    }

  	return -1;
}

int main() {
    std::ifstream ifs("speech.txt");
    std::string T((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    // cout << T << endl;

    vector<char> v;
    for (int c = 32; c <= 122; c++) {
        v.push_back(char(c));
    }

    int size = v.size();

    // float bruteTime = 0.0;
    // float kmpTime = 0.0;
    // float bmhTime = 0.0;
    // float rkTime = 0.0;
    // float bitapTime = 0.0;

    float brute_2 = 0.0;
    float brute_3 = 0.0;
    float brute_4 = 0.0;
    float brute_5 = 0.0;
    float brute_6 = 0.0;
    float brute_7 = 0.0;
    float brute_8 = 0.0;
    float brute_9 = 0.0;
    float brute_10 = 0.0;

    float kmp_2 = 0.0;
    float kmp_3 = 0.0;
    float kmp_4 = 0.0;
    float kmp_5 = 0.0;
    float kmp_6 = 0.0;
    float kmp_7 = 0.0;
    float kmp_8 = 0.0;
    float kmp_9 = 0.0;
    float kmp_10 = 0.0;

    float bmh_2 = 0.0;
    float bmh_3 = 0.0;
    float bmh_4 = 0.0;
    float bmh_5 = 0.0;
    float bmh_6 = 0.0;
    float bmh_7 = 0.0;
    float bmh_8 = 0.0;
    float bmh_9 = 0.0;
    float bmh_10 = 0.0;

    float rk_2 = 0.0;
    float rk_3 = 0.0;
    float rk_4 = 0.0;
    float rk_5 = 0.0;
    float rk_6 = 0.0;
    float rk_7 = 0.0;
    float rk_8 = 0.0;
    float rk_9 = 0.0;
    float rk_10 = 0.0;

    float bitap_2 = 0.0;
    float bitap_3 = 0.0;
    float bitap_4 = 0.0;
    float bitap_5 = 0.0;
    float bitap_6 = 0.0;
    float bitap_7 = 0.0;
    float bitap_8 = 0.0;
    float bitap_9 = 0.0;
    float bitap_10 = 0.0;

    int len_2 = 0;
    int len_3 = 0;
    int len_4 = 0;
    int len_5 = 0;
    int len_6 = 0;
    int len_7 = 0;
    int len_8 = 0;
    int len_9 = 0;
    int len_10 = 0;

    fstream new_file;
    new_file.open("words1.txt", ios::in);

    if (new_file.is_open()) {
      string s;
      int i = 0;
      while (getline(new_file, s)) {
        i++;
        std::cout << i << endl;
        if (s.length() >= 2 && s.length() <= 10) {
          auto start = high_resolution_clock::now();
          brute(T, s);
          auto stop = high_resolution_clock::now();
          auto duration_brute = duration_cast<microseconds>(stop - start);
          
          start = high_resolution_clock::now();
          kmp(T, s);
          stop = high_resolution_clock::now();
          auto duration_kmp = duration_cast<microseconds>(stop - start);

          start = high_resolution_clock::now();
          bmh(T, s);
          stop = high_resolution_clock::now();
          auto duration_bmh = duration_cast<microseconds>(stop - start);

          start = high_resolution_clock::now();
          rk(T, s);
          stop = high_resolution_clock::now();
          auto duration_rk = duration_cast<microseconds>(stop - start);

          start = high_resolution_clock::now();
          bitap(T, s);
          stop = high_resolution_clock::now();
          auto duration_bitap = duration_cast<microseconds>(stop - start);

          if (s.length() == 2) {
            brute_2 += (duration_brute.count() / 1000.0);
            kmp_2 += (duration_kmp.count() / 1000.0);
            bmh_2 += (duration_bmh.count() / 1000.0);
            rk_2 += (duration_rk.count() / 1000.0);
            bitap_2 += (duration_bitap.count() / 1000.0);

            len_2++;
          } else if (s.length() == 3) {
            brute_3 += (duration_brute.count() / 1000.0);
            kmp_3 += (duration_kmp.count() / 1000.0);
            bmh_3 += (duration_bmh.count() / 1000.0);
            rk_3 += (duration_rk.count() / 1000.0);
            bitap_3 += (duration_bitap.count() / 1000.0);

            len_3++;
          } else if (s.length() == 4) {
            brute_4 += (duration_brute.count() / 1000.0);
            kmp_4 += (duration_kmp.count() / 1000.0);
            bmh_4 += (duration_bmh.count() / 1000.0);
            rk_4 += (duration_rk.count() / 1000.0);
            bitap_4 += (duration_bitap.count() / 1000.0);

            len_4++;
          } else if (s.length() == 5) {
            brute_5 += (duration_brute.count() / 1000.0);
            kmp_5 += (duration_kmp.count() / 1000.0);
            bmh_5 += (duration_bmh.count() / 1000.0);
            rk_5 += (duration_rk.count() / 1000.0);
            bitap_5 += (duration_bitap.count() / 1000.0);

            len_5++;
          } else if (s.length() == 6) {
            brute_6 += (duration_brute.count() / 1000.0);
            kmp_6 += (duration_kmp.count() / 1000.0);
            bmh_6 += (duration_bmh.count() / 1000.0);
            rk_6 += (duration_rk.count() / 1000.0);
            bitap_6 += (duration_bitap.count() / 1000.0);

            len_6++;
          } else if (s.length() == 7) {
            brute_7 += (duration_brute.count() / 1000.0);
            kmp_7 += (duration_kmp.count() / 1000.0);
            bmh_7 += (duration_bmh.count() / 1000.0);
            rk_7 += (duration_rk.count() / 1000.0);
            bitap_7 += (duration_bitap.count() / 1000.0);

            len_7++;
          } else if (s.length() == 8) {
            brute_8 += (duration_brute.count() / 1000.0);
            kmp_8 += (duration_kmp.count() / 1000.0);
            bmh_8 += (duration_bmh.count() / 1000.0);
            rk_8 += (duration_rk.count() / 1000.0);
            bitap_8 += (duration_bitap.count() / 1000.0);

            len_8++;
          } else if (s.length() == 9) {
            brute_9 += (duration_brute.count() / 1000.0);
            kmp_9 += (duration_kmp.count() / 1000.0);
            bmh_9 += (duration_bmh.count() / 1000.0);
            rk_9 += (duration_rk.count() / 1000.0);
            bitap_9 += (duration_bitap.count() / 1000.0);

            len_9++;
          } else if (s.length() == 10) {
            brute_10 += (duration_brute.count() / 1000.0);
            kmp_10 += (duration_kmp.count() / 1000.0);
            bmh_10 += (duration_bmh.count() / 1000.0);
            rk_10 += (duration_rk.count() / 1000.0);
            bitap_10 += (duration_bitap.count() / 1000.0);

            len_10++;
          }
        }
      }
    }

    std::cout << "Average Brute Force time for m = 2 = " << brute_2 / (float)(len_2 * 1.0) << " milliseconds" << endl;
    std::cout << "Average Brute Force time for m = 3 = " << brute_3 / (float)(len_3 * 1.0) << " milliseconds" << endl;
    std::cout << "Average Brute Force time for m = 4 = " << brute_4 / (float)(len_4 * 1.0) << " milliseconds" << endl;
    std::cout << "Average Brute Force time for m = 5 = " << brute_5 / (float)(len_5 * 1.0) << " milliseconds" << endl;
    std::cout << "Average Brute Force time for m = 6 = " << brute_6 / (float)(len_6 * 1.0) << " milliseconds" << endl;
    std::cout << "Average Brute Force time for m = 7 = " << brute_7 / (float)(len_7 * 1.0) << " milliseconds" << endl;
    std::cout << "Average Brute Force time for m = 8 = " << brute_8 / (float)(len_8 * 1.0) << " milliseconds" << endl;
    std::cout << "Average Brute Force time for m = 9 = " << brute_9 / (float)(len_9 * 1.0) << " milliseconds" << endl;
    std::cout << "Average Brute Force time for m = 10 = " << brute_10 / (float)(len_10 * 1.0) << " milliseconds" << endl;

    std::cout << "Average KMP time for m = 2 = " << kmp_2 / (float)(len_2 * 1.0) << " milliseconds" << endl;
    std::cout << "Average KMP time for m = 3 = " << kmp_3 / (float)(len_3 * 1.0) << " milliseconds" << endl;
    std::cout << "Average KMP time for m = 4 = " << kmp_4 / (float)(len_4 * 1.0) << " milliseconds" << endl;
    std::cout << "Average KMP time for m = 5 = " << kmp_5 / (float)(len_5 * 1.0) << " milliseconds" << endl;
    std::cout << "Average KMP time for m = 6 = " << kmp_6 / (float)(len_6 * 1.0) << " milliseconds" << endl;
    std::cout << "Average KMP time for m = 7 = " << kmp_7 / (float)(len_7 * 1.0) << " milliseconds" << endl;
    std::cout << "Average KMP time for m = 8 = " << kmp_8 / (float)(len_8 * 1.0) << " milliseconds" << endl;
    std::cout << "Average KMP time for m = 9 = " << kmp_9 / (float)(len_9 * 1.0) << " milliseconds" << endl;
    std::cout << "Average KMP time for m = 10 = " << kmp_10 / (float)(len_10 * 1.0) << " milliseconds" << endl;

    std::cout << "Average BMH time for m = 2 = " << bmh_2 / (float)(len_2 * 1.0) << " milliseconds" << endl;
    std::cout << "Average BMH time for m = 3 = " << bmh_3 / (float)(len_3 * 1.0) << " milliseconds" << endl;
    std::cout << "Average BMH time for m = 4 = " << bmh_4 / (float)(len_4 * 1.0) << " milliseconds" << endl;
    std::cout << "Average BMH time for m = 5 = " << bmh_5 / (float)(len_5 * 1.0) << " milliseconds" << endl;
    std::cout << "Average BMH time for m = 6 = " << bmh_6 / (float)(len_6 * 1.0) << " milliseconds" << endl;
    std::cout << "Average BMH time for m = 7 = " << bmh_7 / (float)(len_7 * 1.0) << " milliseconds" << endl;
    std::cout << "Average BMH time for m = 8 = " << bmh_8 / (float)(len_8 * 1.0) << " milliseconds" << endl;
    std::cout << "Average BMH time for m = 9 = " << bmh_9 / (float)(len_9 * 1.0) << " milliseconds" << endl;
    std::cout << "Average BMH time for m = 10 = " << bmh_10 / (float)(len_10 * 1.0) << " milliseconds" << endl;

    std::cout << "Average RK time for m = 2 = " << rk_2 / (float)(len_2 * 1.0) << " milliseconds" << endl;
    std::cout << "Average RK time for m = 3 = " << rk_3 / (float)(len_3 * 1.0) << " milliseconds" << endl;
    std::cout << "Average RK time for m = 4 = " << rk_4 / (float)(len_4 * 1.0) << " milliseconds" << endl;
    std::cout << "Average RK time for m = 5 = " << rk_5 / (float)(len_5 * 1.0) << " milliseconds" << endl;
    std::cout << "Average RK time for m = 6 = " << rk_6 / (float)(len_6 * 1.0) << " milliseconds" << endl;
    std::cout << "Average RK time for m = 7 = " << rk_7 / (float)(len_7 * 1.0) << " milliseconds" << endl;
    std::cout << "Average RK time for m = 8 = " << rk_8 / (float)(len_8 * 1.0) << " milliseconds" << endl;
    std::cout << "Average RK time for m = 9 = " << rk_9 / (float)(len_9 * 1.0) << " milliseconds" << endl;
    std::cout << "Average RK time for m = 10 = " << rk_10 / (float)(len_10 * 1.0) << " milliseconds" << endl;

    std::cout << "Average Bitap time for m = 2 = " << bitap_2 / (float)(len_2 * 1.0) << " milliseconds" << endl;
    std::cout << "Average Bitap time for m = 3 = " << bitap_3 / (float)(len_3 * 1.0) << " milliseconds" << endl;
    std::cout << "Average Bitap time for m = 4 = " << bitap_4 / (float)(len_4 * 1.0) << " milliseconds" << endl;
    std::cout << "Average Bitap time for m = 5 = " << bitap_5 / (float)(len_5 * 1.0) << " milliseconds" << endl;
    std::cout << "Average Bitap time for m = 6 = " << bitap_6 / (float)(len_6 * 1.0) << " milliseconds" << endl;
    std::cout << "Average Bitap time for m = 7 = " << bitap_7 / (float)(len_7 * 1.0) << " milliseconds" << endl;
    std::cout << "Average Bitap time for m = 8 = " << bitap_8 / (float)(len_8 * 1.0) << " milliseconds" << endl;
    std::cout << "Average Bitap time for m = 9 = " << bitap_9 / (float)(len_9 * 1.0) << " milliseconds" << endl;
    std::cout << "Average Bitap time for m = 10 = " << bitap_10 / (float)(len_10 * 1.0) << " milliseconds" << endl;

    // for (int i = 2; i <= 10; i++) {
    //     for (int j = 1; j <= 10000; j++) {
    //         string P = "";
    //         for (int k = 1; k <= i; k++) {
    //             int randNum = getRand(1, size) - 1;
    //             P.push_back(v[randNum]);
    //         }
    //         auto start = high_resolution_clock::now();
    //         brute(T, P);
    //         auto stop = high_resolution_clock::now();
    //         auto duration = duration_cast<microseconds>(stop - start);
    //         bruteTime += (duration.count() / 1000.0);
    //     }
    //     cout << "Average Brute Force time for m = " << i << " = " << bruteTime / 10000.0 << " milliseconds" << endl;
    // }

    // for (int i = 2; i <= 10; i++) {
    //     for (int j = 1; j <= 10000; j++) {
    //         string P = "";
    //         for (int k = 1; k <= i; k++) {
    //             int randNum = getRand(1, size) - 1;
    //             P.push_back(v[randNum]);
    //         }
    //         auto start = high_resolution_clock::now();
    //         kmp(T, P);
    //         auto stop = high_resolution_clock::now();
    //         auto duration = duration_cast<microseconds>(stop - start);
    //         kmpTime += (duration.count() / 1000.0);
    //     }
    //     cout << "Average KMP time for m = " << i << " = " << kmpTime / 10000.0 << " milliseconds" << endl;
    // }

    // for (int i = 2; i <= 10; i++) {
    //     for (int j = 1; j <= 10000; j++) {
    //         string P = "";
    //         for (int k = 1; k <= i; k++) {
    //             int randNum = getRand(1, size) - 1;
    //             P.push_back(v[randNum]);
    //         }
    //         auto start = high_resolution_clock::now();
    //         bmh(T, P);
    //         auto stop = high_resolution_clock::now();
    //         auto duration = duration_cast<microseconds>(stop - start);
    //         bmhTime += (duration.count() / 1000.0);
    //     }
    //     cout << "Average BMH time for m = " << i << " = " << bmhTime / 10000.0 << " milliseconds" << endl;
    // }

    // for (int i = 2; i <= 10; i++) {
    //     for (int j = 1; j <= 10000; j++) {
    //         string P = "";
    //         for (int k = 1; k <= i; k++) {
    //             int randNum = getRand(1, size) - 1;
    //             P.push_back(v[randNum]);
    //         }
    //         auto start = high_resolution_clock::now();
    //         rk(T, P);
    //         auto stop = high_resolution_clock::now();
    //         auto duration = duration_cast<microseconds>(stop - start);
    //         rkTime += (duration.count() / 1000.0);
    //     }
    //     cout << "Average RK time for m = " << i << " = " << rkTime / 10000.0 << " milliseconds" << endl;
    // }

    // for (int i = 2; i <= 10; i++) {
    //     for (int j = 1; j <= 10000; j++) {
    //         string P = "";
    //         for (int k = 1; k <= i; k++) {
    //             int randNum = getRand(1, size) - 1;
    //             P.push_back(v[randNum]);
    //         }
    //         auto start = high_resolution_clock::now();
    //         bitap(T, P);
    //         auto stop = high_resolution_clock::now();
    //         auto duration = duration_cast<microseconds>(stop - start);
    //         bitapTime += (duration.count() / 1000.0);
    //     }
    //     cout << "Average Bitap time for m = " << i << " = " << bitapTime / 10000.0 << " milliseconds" << endl;
    // }
}