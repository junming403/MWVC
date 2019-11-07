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
int src[600010], des[600010];
bool choose[4010] = {0}, optimal_choose[4010] = {0};
vector<vi> AL;
int optimal = 0, V, E, cur;

chrono::time_point<chrono::steady_clock> start;

double score[4010];
double wconfig[4010] = {1}, dy_weight[600010] = {1};
bool isCoverAll = true;
int cover[600010] = {0};
unordered_set<int> tabu;
unordered_set<int> uncovered;


void removeVertex(int idx) {
	choose[idx] = false;
	score[idx] *= -1;
	wconfig[idx] = 0;
	for (int i = 0; i < AL[idx].size(); i++) {
		int neigh = src[AL[idx][i]] == idx ? des[AL[idx][i]] : src[AL[idx][i]];
		if (choose[idx] == choose[neigh]) {
			score[neigh] -= dy_weight[AL[idx][i]] / w[neigh];
		} else{
			score[neigh] += dy_weight[AL[idx][i]] / w[neigh];
		}
		wconfig[neigh] = 1;
		cover[AL[idx][i]]--;
		if (cover[AL[idx][i]] == 0) {
			uncovered.insert(AL[idx][i]);
		}
	}
}

void addVertex(int idx) {
	choose[idx] = true;
	score[idx] *= -1;
	for (int i = 0; i < AL[idx].size(); i++) {
		int neigh = src[AL[idx][i]] == idx ? des[AL[idx][i]] : src[AL[idx][i]];
		if (choose[idx] == choose[neigh]) {
			score[neigh] -= dy_weight[AL[idx][i]] / w[neigh];
		} else{
			score[neigh] += dy_weight[AL[idx][i]] / w[neigh];
		}
		wconfig[neigh] = 1;
		cover[AL[idx][i]]++;
		if (cover[AL[idx][i]] == 1) {
			uncovered.erase(AL[idx][i]);
		}
	}	
}


void initial();
void dymwvc() {
	chrono::time_point<chrono::steady_clock> end;
	while(1) {
		end = chrono::steady_clock::now();
		if (((chrono::duration<double>)(end-start)).count() >= 1.99) break;

		while (uncovered.empty()) {
			memcpy(optimal_choose, choose, sizeof(bool) * V);
			optimal = cur;

			double bscore = -1e9;
			int idx;
			for (int i = 0; i < V; i++) {
				if (choose[i] && score[i] > bscore) {
					bscore = score[i];
					idx = i;
				}
			}
			removeVertex(idx);
			cur -= w[idx];
		}
		double bscore = -1e9;
		int idx;
		for (int i = 0; i < V; i++) {
			if (choose[i] && score[i] > bscore && tabu.find(i) == tabu.end()) {
				bscore = score[i];
				idx = i;
			}
		}
		removeVertex(idx);
		cur -= w[idx];
		tabu.clear();

		while (!uncovered.empty()) {
			double bscore = -1e9;
			int idx;
			for (int i = 0; i < V; i++) {
				if (!choose[i] && wconfig[i] == 1 && score[i] > bscore) {
					bscore = score[i];
					idx = i;
				}
			}
			if (cur + w[idx] > optimal) break;
			addVertex(idx);
			cur = cur + w[idx];
			for (auto edge : uncovered) {
				dy_weight[edge]++;
				wconfig[src[edge]] = 1;
				wconfig[des[edge]] = 1;
			}
			tabu.insert(idx);
		}
	}

}

vector<const char*> tests = {
	// "data/frb35-17-1.mis",
	// "data/frb56-25-1.mis",
	"data/frb59-26-1.mis"
};


int main() {
	int total = 0;
	for (auto test: tests) {
		AL.clear();
		memset(cover, 0, sizeof(cover));
		memset(choose, false, sizeof(choose));
		memset(optimal_choose, false, sizeof(optimal_choose));
		start = chrono::steady_clock::now();
		freopen(test, "r", stdin);
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
		for (int i = 0; i < V; i++) {
			score[i] = AL[i].size();
		}
		initial();
		for (int j = 0; j < V; j++) {
			if (choose[j]) {
				score[j] *= -1;
				for (int i = 0; i < AL[j].size(); i++) {
					int neigh = src[AL[j][i]] == j ? des[AL[j][i]] : src[AL[j][i]];
					if (choose[j] == choose[neigh]) {
						score[neigh] -= dy_weight[AL[j][i]] / w[neigh];
					} else{
						score[neigh] += dy_weight[AL[j][i]] / w[neigh];
					}
				}
			}
		}
		cur = optimal;
		cout << "INITIAL: " << optimal << endl;
		dymwvc();
		cout << optimal << endl;
	}

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
				cover[nb]++;
			}
		}
	}
	memcpy(optimal_choose, choose, sizeof(bool) * V);
}
