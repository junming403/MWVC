#include <cstdio>
#include <queue>
#include <cstring>
#include <random>
#include <iostream>
#include <vector>
#include <chrono>
#include <unordered_set>
using namespace std;
typedef vector<int> vi;
int w[4010], cp_w[4010];
int counter[600010] = {0};
int src[600010], des[600010];
bool choose[4010] = {0}, optimal_choose[4010] = {0};
vector<vi> AL;
int optimal = 0, V, E, cur;
int alpha = 5;

double dy_weight[600010] = {1};
double loss[4010], gain[4010];
int valid_score[4010];

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
		// double div = ((chrono::duration<double>)(end-start)).count() * 9.0;
		// int n_candidate = max(V / (20 - div), 1.0);
		int n_candidate = V / 7;
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
		// cout << "cost is " << cur << endl;
		// for (int i = 0; i < V; i++) {
		// 	if (choose[i]) cout << i << " ";
		// }
		// cout << endl;

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
		int v = dychoose(cur_count);
		choose[v] = false;
		gain[u] = 0;
		gain[v] = 0;
		vector<int> nr;
		for (int i = 0; i < AL[u].size(); i++) {
			int neigh = src[AL[u][i]] == u ? des[AL[u][i]] : src[AL[u][i]];
			nr.push_back(neigh);
			if (counter[AL[u][i]] != 1) {
				loss[neigh] += dy_weight[AL[u][i]];
			} else {
				gain[u] += dy_weight[AL[u][i]];
				gain[neigh] += dy_weight[AL[u][i]];
			}
			counter[AL[u][i]]--;
			if (counter[AL[u][i]] == 0) uncovered_set.insert(AL[u][i]);
		}
		for (int i = 0; i < AL[v].size(); i++) {
			int neigh = src[AL[v][i]] == v ? des[AL[v][i]] : src[AL[v][i]];
			nr.push_back(neigh);
			if (counter[AL[v][i]] != 1) {
				loss[neigh] += dy_weight[AL[v][i]];
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
			int maxGain = -1, idx;
			for (int i = 0; i < nr.size(); i++) {
				if (!choose[nr[i]] && gain[nr[i]] > maxGain) {
					maxGain = gain[nr[i]];
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
					loss[neigh]-=dy_weight[AL[rm][j]];
				}
				counter[AL[rm][j]]++;
				if (counter[AL[rm][j]] == 1) uncovered_set.erase(AL[rm][j]);
			}
			for (auto e : uncovered_set) {
				dy_weight[e]++;
				gain[src[e]]++;
				gain[des[e]]++;
			}
			for (int j = 0; j < AL[rm].size(); j++) {
				int neigh = src[AL[rm][j]] == rm ? des[AL[rm][j]] : src[AL[rm][j]];
				if (!choose[neigh]) continue;
				bool flag = true;
				for (int k = 0; k < AL[neigh].size(); k++) {
					if (counter[AL[neigh][k]] == 1) flag = false;
				}
				if (flag) {
					int uu = neigh;
					choose[uu] = false;
					costChange -= w[uu];
					gain[uu] = 0;
					for (int i = 0; i < AL[uu].size(); i++) {
						int nei = src[AL[uu][i]] == uu ? des[AL[uu][i]] : src[AL[uu][i]];
						loss[nei]+=dy_weight[AL[uu][i]];
						counter[AL[uu][i]]--;
					}
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


int main() {
	start = chrono::steady_clock::now();
	freopen("data/frb59-26-1.mis", "r", stdin);
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
	cout << "2-OPT: " << optimal << endl;
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