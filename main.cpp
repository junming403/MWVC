#include <cstdio>
#include <queue>
#include <cstring>
#include <vector>

typedef vector<int> vi;

using namespace std;
int w[4010], cp_w[4010];
int src[600010], des[600010];
int counter[600010] = {0};
vector<vi> AL;
int optimal;
bool choose[4010] = {0};

void getNeighbour() {

}

void initial(int V, int E) {
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
				counter[i]++;
			}
		}
	}
}

int main() {
	//freopen("input.txt", "r", stdin);
	int V, E;
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
	initial(V, E);
	
	return 0;
}
