#include <cstdio>
#include <queue>
#include <cstring>
#include <random>
#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
typedef vector<int> vi;
int w[4010], cp_w[4010];
int counter[1000010] = {0};
int src[1000010], des[1000010];
bool choose[4010] = {0}, optimal_choose[4010] = {0};
vector<vi> AL;
int optimal = 0, V, E, cur;
int alpha = 3;

double loss[4010];
int valid_score[4010];
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
		double div = ((chrono::duration<double>)(end-start)).count() * 19.0;
		int n_candidate = max(V / (40 - div), 1.0);
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
		for (int i = 0; i < AL[u].size(); i++) {
			int neigh = src[AL[u][i]] == u ? des[AL[u][i]] : src[AL[u][i]];
			if (choose[neigh]) {
				loss[neigh]++;
				valid_score[neigh] += w[u]; 
			}
			counter[AL[u][i]]--;
		}

		int v = dychoose(cur_count);
		choose[v] = false;
		for (int i = 0; i < AL[v].size(); i++) {
			int neigh = src[AL[v][i]] == v ? des[AL[v][i]] : src[AL[v][i]];
			if (choose[neigh]) {
				loss[neigh]++;
				valid_score[neigh] += w[v]; 
			}
			counter[AL[v][i]]--;
		}

		int costChange = -1 * (w[u] + w[v]);
		for (int i = 0; i < AL[u].size(); i++) {
			if (counter[AL[u][i]] == 0) {
				int rm = src[AL[u][i]] == u ? des[AL[u][i]] : src[AL[u][i]];
				choose[rm] = true;
				costChange += w[rm];
				loss[rm] = 0;
				valid_score[rm] = -1 * w[rm];
				for (int j = 0; j < AL[rm].size(); j++) {
					int neigh = src[AL[rm][j]] == rm ? des[AL[rm][j]] : src[AL[rm][j]];
					if (counter[AL[rm][j]] == 0) {
						loss[rm]++;
						valid_score[rm] += w[neigh];
					} else {
						loss[neigh]--;
						valid_score[neigh] -= w[rm];
						if (loss[neigh] == 0) {
							choose[neigh] = false;
							costChange -= w[neigh];
							for (int k = 0; k < AL[neigh].size(); k++) {
								int nei = src[AL[neigh][k]] == neigh ? des[AL[neigh][k]] : src[AL[neigh][k]];
								loss[nei]++;
								valid_score[nei] += w[neigh];
								counter[AL[neigh][k]]--;
							}
						}
					}
					counter[AL[rm][j]]++;
				}
			}
		}
		for (int i = 0; i < AL[v].size(); i++) {
			if (counter[AL[v][i]] == 0) {
				int rm = src[AL[v][i]] == v ? des[AL[v][i]] : src[AL[v][i]];
				choose[rm] = true;
				costChange += w[rm];
				loss[rm] = 0;
				valid_score[rm] = -1 * w[rm];
				for (int j = 0; j < AL[rm].size(); j++) {
					int neigh = src[AL[rm][j]] == rm ? des[AL[rm][j]] : src[AL[rm][j]];
					if (counter[AL[rm][j]] == 0) {
						loss[rm]++;
						valid_score[rm] += w[neigh];
					} else {
						loss[neigh]--;
						valid_score[neigh] -= w[rm];
						if (loss[neigh] == 0) {
							choose[neigh] = false;
							costChange -= w[neigh];
							for (int k = 0; k < AL[neigh].size(); k++) {
								int nei = src[AL[neigh][k]] == neigh ? des[AL[neigh][k]] : src[AL[neigh][k]];
								loss[nei]++;
								valid_score[nei] += w[neigh];
								counter[AL[neigh][k]]--;
							}
						}
					}
					counter[AL[rm][j]]++;
				}
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

// vector<const char*> dataset = {
// 	"./data/san200_0.9_1.txt",
// 	"./data/san1000.txt",
// 	"./data/C2000.5.txt",
// 	"./data/p_hat1500-2.txt",
// 	"./data/brock800_4.txt",
// 	"./data/frb59-26-1.mis",
// 	"./data/DSJC1000_5.txt",
// };

int main() {
	start = chrono::steady_clock::now();
	// freopen(dataset[2], "r", stdin);
	scanf("%d%d", &V, &E);
	AL.assign(V, vi());
	for (int i = 0; i < V; i++) {
		scanf("%d", &w[i]);
		cp_w[i] = w[i];
	}
	for (int i = 0; i < E; i++) {
		scanf("%d%d", &src[i], &des[i]);
		AL[src[i]].emplace_back(i);
		AL[des[i]].emplace_back(i);
	}
	initial();
	cur = optimal;

	for (int j = 0; j < V; j++) {
		if (choose[j]) {
			loss[j] = 0;
			valid_score[j] = -1 * w[j];
			for (int i = 0; i < AL[j].size(); i++) {
				if (counter[AL[j][i]] == 1) {
					loss[j]++;
					valid_score[j] += w[src[AL[j][i]] == j ? des[AL[j][i]] : src[AL[j][i]]];
				}
			}
		}
	}
	dymwvc();
	printf("%d\n", optimal);
	for (int i = 0; i < V; i++) {
		if (optimal_choose[i]) printf("%d ", i);
	}
	printf("\n");
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
