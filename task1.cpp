#include <iostream>
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

int main()
{
    vector<char> v;
    for (int c = 32; c <= 122; c++) {
        v.push_back(char(c));
    }

    int size = v.size();

    string T = "";
    for (int i = 1; i <= 10000; i++) {
        int randNum = getRand(1, size) - 1;
        T.push_back(v[randNum]);
    }

    float bruteTime = 0.0;
    float kmpTime = 0.0;
    float bmhTime = 0.0;
    float rkTime = 0.0;
    float bitapTime = 0.0;

    for (int i = 2; i <= 10; i++) {
        for (int j = 1; j <= 100000; j++) {
            string P = "";
            for (int k = 1; k <= i; k++) {
                int randNum = getRand(1, size) - 1;
                P.push_back(v[randNum]);
            }
            auto start = high_resolution_clock::now();
            brute(T, P);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            bruteTime += (duration.count() / 1000.0);
        }
        cout << "Average Brute Force time for m = " << i << " = " << bruteTime / 100000.0 << " milliseconds" << endl;
    }

    for (int i = 2; i <= 10; i++) {
        for (int j = 1; j <= 100000; j++) {
            string P = "";
            for (int k = 1; k <= i; k++) {
                int randNum = getRand(1, size) - 1;
                P.push_back(v[randNum]);
            }
            auto start = high_resolution_clock::now();
            kmp(T, P);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            kmpTime += (duration.count() / 1000.0);
        }
        cout << "Average KMP time for m = " << i << " = " << kmpTime / 100000.0 << " milliseconds" << endl;
    }

    for (int i = 2; i <= 10; i++) {
        for (int j = 1; j <= 100000; j++) {
            string P = "";
            for (int k = 1; k <= i; k++) {
                int randNum = getRand(1, size) - 1;
                P.push_back(v[randNum]);
            }
            auto start = high_resolution_clock::now();
            bmh(T, P);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            bmhTime += (duration.count() / 1000.0);
        }
        cout << "Average BMH time for m = " << i << " = " << bmhTime / 100000.0 << " milliseconds" << endl;
    }

    for (int i = 2; i <= 10; i++) {
        for (int j = 1; j <= 100000; j++) {
            string P = "";
            for (int k = 1; k <= i; k++) {
                int randNum = getRand(1, size) - 1;
                P.push_back(v[randNum]);
            }
            auto start = high_resolution_clock::now();
            rk(T, P);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            rkTime += (duration.count() / 1000.0);
        }
        cout << "Average RK time for m = " << i << " = " << rkTime / 100000.0 << " milliseconds" << endl;
    }

    for (int i = 2; i <= 10; i++) {
        for (int j = 1; j <= 100000; j++) {
            string P = "";
            for (int k = 1; k <= i; k++) {
                int randNum = getRand(1, size) - 1;
                P.push_back(v[randNum]);
            }
            auto start = high_resolution_clock::now();
            bitap(T, P);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            bitapTime += (duration.count() / 1000.0);
        }
        cout << "Average Bitap time for m = " << i << " = " << bitapTime / 100000.0 << " milliseconds" << endl;
    }

}