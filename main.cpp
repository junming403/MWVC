#include <cstdio>
#include <queue>
#include <cstring>
#include <random>
#include <vector>
#include <chrono>
using namespace std;

typedef vector<int> vi;

int w[4010], cp_w[4010];
int src[600010], des[600010];
int counter[600010] = {0};
vector<vi> AL;
int optimal;
bool choose[4010] = {0};
int V, E;

void getNeighbour() {

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
}

int main() {
	chrono::time_point<chrono::steady_clock> start, end;
	start = chrono::steady_clock::now();
	freopen("data/frb56-25-1.mis", "r", stdin);
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
	// mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
	// for (int i = 0; i < 10; i++)
	// printf("%d\n", rng()%100);
	// uniform_int_distribution<int> dist(0, 100);
	// random_device rd;
	// for (int i = 0; i < 10; i++)
	// printf("%d\n", dist(rd));
	// while (1) {
	// 	// Search
	// 	end = chrono::steady_clock::now();
	// 	if (((chrono::duration<double>)(end-start)).count() >= 1.99) break;
	// }
	printf("%d\n", optimal);
	// for (int i = 0; i < V; i++) {
	// 	if (choose[i]) printf("%d ", i);
	// }
	printf("\n");
	return 0;
}
