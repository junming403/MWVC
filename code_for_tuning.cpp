#include <cstdio>
#include <queue>
#include <cstring>
#include <random>
#include <iostream>
#include <vector>
#include <chrono>
#include <stdlib.h>
#include <unordered_set>
// #include <bits/stdc++.h>
using namespace std;
const int ELEN = 600010;
typedef vector<int> vi;
int w[4010], cp_w[4010];
int counter[ELEN] = {0};
int src[ELEN], des[ELEN];
bool choose[4010] = {0}, optimal_choose[4010] = {0};
vector<vi> AL;
int optimal = 0, V, E, cur;

int alpha = 10;
double dy_start = 0;
double dy_end = 0;

double dy_weight[ELEN];
double loss[4010] = {0}, gain[4010] ={0};
int valid_score[4010] ={0};

unordered_set<int> uncovered_set;

chrono::time_point<chrono::steady_clock> start;
void initial();
int dychoose(int &cur_count) {
	if (cur_count < alpha) {
		int min_valid_score = 2e9, idx = 0;
		for (int i = 0; i < V; i++) {
			if (choose[i]) {
				int cur = valid_score[i];
				if (cur < min_valid_score) {
					min_valid_score = cur;
					idx = i;
				}
			}
		}
		return idx;
	} else {
		double min_loss = 2e9;
		int idx = 0;
		mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
		chrono::time_point<chrono::steady_clock> end;
		end = chrono::steady_clock::now();
		int n_candidate;
		double div = ((chrono::duration<double>)(end-start)).count() * ((dy_end-dy_start) / 2.0) + dy_start;
		n_candidate = max(V / div, 1.0);
		while (n_candidate > 0) {
			int i = rng() % V;
			if (choose[i]) {
				n_candidate--;
				double cur = loss[i] / w[i];
				if (cur < min_loss) {
					min_loss = cur;
					idx = i;
				}
			}
		}
		cur_count = 0;
		return idx;
	}
}
void dymwvc() {
	int cur_count = 0;
	chrono::time_point<chrono::steady_clock> end;
	while(1) {
		end = chrono::steady_clock::now();
		if (((chrono::duration<double>)(end-start)).count() >= 1.99) break;

		double min_loss = 2e9, idx = 0;
		for (int i = 0; i < V; i++) {
			if (choose[i]) {
				double cur = loss[i] / w[i];
				if (cur < min_loss) {
					min_loss = cur;
					idx = i;
				}
			}
		}
		int u = idx;
		choose[u] = false;
		gain[u] = 0;
		vector<int> nr;
		for (int i = 0; i < AL[u].size(); i++) {
			int neigh = src[AL[u][i]] == u ? des[AL[u][i]] : src[AL[u][i]];
			nr.push_back(neigh);
			if (counter[AL[u][i]] != 1) {
				loss[neigh] += dy_weight[AL[u][i]];
				valid_score[neigh] += w[u]; 
			} else {
				gain[u] += dy_weight[AL[u][i]];
				gain[neigh] += dy_weight[AL[u][i]];
			}
			counter[AL[u][i]]--;
			if (counter[AL[u][i]] == 0) uncovered_set.insert(AL[u][i]);
		}

		int v = dychoose(cur_count);
		choose[v] = false;
		gain[v] = 0;
		for (int i = 0; i < AL[v].size(); i++) {
			int neigh = src[AL[v][i]] == v ? des[AL[v][i]] : src[AL[v][i]];
			nr.push_back(neigh);
			if (counter[AL[v][i]] != 1) {
				loss[neigh] += dy_weight[AL[v][i]];
				valid_score[neigh] += w[v];
			} else {
				gain[v] += dy_weight[AL[v][i]];
				gain[neigh] += dy_weight[AL[v][i]];
			}
			counter[AL[v][i]]--;
			if (counter[AL[v][i]] == 0) uncovered_set.insert(AL[v][i]);
		}
		int costChange = -1 * (w[u] + w[v]);

		sort(nr.begin(), nr.end());
		nr.erase(unique(nr.begin(), nr.end()), nr.end());
		while (!uncovered_set.empty()) {
			double maxGain = -1;
			int idx;
			for (int i = 0; i < nr.size(); i++) {
				double curgain = gain[nr[i]] / w[nr[i]];
				if (!choose[nr[i]] && curgain > maxGain) {
					maxGain = curgain;
					idx = i;
				}
			}
			int rm = nr[idx];
			choose[rm] = true;
			costChange += w[rm];
			loss[rm] = 0;
			valid_score[rm] = -1 * w[rm];
			for (int j = 0; j < AL[rm].size(); j++) {
				int neigh = src[AL[rm][j]] == rm ? des[AL[rm][j]] : src[AL[rm][j]];
				if (counter[AL[rm][j]] == 0) {
					loss[rm]+=dy_weight[AL[rm][j]];
					gain[neigh]-=dy_weight[AL[rm][j]];
					valid_score[rm] += w[neigh];
				} else {
					valid_score[neigh]-=w[rm];
					loss[neigh]-=dy_weight[AL[rm][j]];
					if (loss[neigh] == 0) {
						choose[neigh] = false;
						gain[neigh] = 0;
						costChange -= w[neigh];
						for (int k = 0; k < AL[neigh].size(); k++) {
							int nei = src[AL[neigh][k]] == neigh ? des[AL[neigh][k]] : src[AL[neigh][k]];
							loss[nei]+=dy_weight[AL[neigh][k]];
							valid_score[nei] += w[neigh];
							counter[AL[neigh][k]]--;
						}
					}
				}
				counter[AL[rm][j]]++;
				if (counter[AL[rm][j]] == 1) uncovered_set.erase(AL[rm][j]);
			}
			for (auto e : uncovered_set) {
				dy_weight[e]++;
				gain[src[e]]++;
				gain[des[e]]++;
			}
		}
		cur = cur + costChange;
		if (cur < optimal) {
			optimal = cur;
			memcpy(optimal_choose, choose, sizeof(bool) * V);
		} else {
			cur_count++;
		}
	}
}

vector<const char*> dataset = {
	//"./test_case/test1.txt",
	//"./test_case/test2.txt",
	//"./test_case/test3.txt",
	"./test_case/test4.txt",
	"./test_case/test5.txt",
	"./test_case/test6.txt",
	"./test_case/test7.txt",
	"./test_case/test8.txt",
	//"./test_case/test9.txt",
	//"./test_case/test10.txt",
	//"./test_case/frb100-40.txt", // 15
};


int main(int argc, char** argv) {
	int fileId = atoi(argv[1]);
	alpha = atoi(argv[2]);
	dy_start = stod(argv[3]);
	dy_end = stod(argv[4]);
	start = chrono::steady_clock::now();
	freopen(dataset[fileId], "r", stdin);
	scanf("%d%d", &V, &E);
	AL.assign(V, vi());
	int sum = 0;
	for (int i = 0; i < V; i++) {
		scanf("%d", &w[i]);
		sum += w[i];
		cp_w[i] = w[i];
	}

	for (int i = 0; i < E; i++) {
		scanf("%d%d", &src[i], &des[i]);
		AL[src[i]].emplace_back(i);
		AL[des[i]].emplace_back(i);
		dy_weight[i] = 1;
	}
	initial();
	cur = optimal;

	for (int j = 0; j < V; j++) {
		if (choose[j]) {
			loss[j] = 0;
			valid_score[j] = -1 * w[j];
			for (int i = 0; i < AL[j].size(); i++) {
				if (counter[AL[j][i]] == 1) {
					loss[j] += dy_weight[AL[j][i]];
					valid_score[j] += w[src[AL[j][i]] == j ? des[AL[j][i]] : src[AL[j][i]]];
				}
			}
		} else {
			gain[j] = 0;
			for (int i = 0; i < AL[j].size(); i++) {
				if (counter[AL[j][i]] == 0) gain[j] += dy_weight[AL[j][i]];
			}
		}
	}
	dymwvc();
	printf("%d\n", optimal);
	// for (int i = 0; i < V; i++) {
	// 	if (optimal_choose[i]) printf("%d ", i);
	// }
	// printf("\n");
	return 0;
}


void initial() {
	for (int i = 0; i < E; i++) {
		int small = cp_w[src[i]] <= cp_w[des[i]] ? src[i] : des[i];
		int e = cp_w[small];
		cp_w[src[i]] -= e;
		cp_w[des[i]] -= e;
	}
	optimal = 0;
	for (int i = 0; i < V; i++) {
		if (cp_w[i] == 0) {
			choose[i] = 1;
			optimal += w[i];
			for (int nb: AL[i]) {
				counter[nb]++;
			}
		}
	}
	memcpy(optimal_choose, choose, sizeof(bool) * V);
}