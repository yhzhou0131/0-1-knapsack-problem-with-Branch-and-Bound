#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;

struct item {
	int value, weight;
	item() { value = 0; weight = 0; };
	item(int v, int w) { value = v; weight = w; };
};

struct node {
	int profit;
	int weight;
	int level;
	int ub;
	node() { profit = 0; weight = 0; level = 0; ub = 0; };
	node(int p, int w, int l, int u) { profit = p; weight = w; level = l; ub = u; };
};

item* itemList;

int upperBound(node& cur, int& n, int& W)
{
	if (cur.weight > W)
		return -1; // infeasible

	int curWeight = cur.weight;
	double ub = cur.profit;
	for (int i = cur.level + 1; i < n; ++i) {
		if (curWeight + itemList[i].weight > W)
		{
			ub += (double(W) - double(curWeight)) / double(itemList[i].weight) * double(itemList[i].value);
			break;
		}
		else
		{
			ub += itemList[i].value;
			curWeight += itemList[i].weight;
		}

	}
	int res = floor(ub);
	return res;
}

int KP(int& W, int& n)
{
	struct cmp {
		bool operator() (node a, node b) {
			return a.ub < b.ub;
		}
	};
	priority_queue <node, vector<node>, cmp> pq;
	node cur(0, 0, -1, 0); // dummy node
	cur.ub = upperBound(cur, n, W);
	double globalUpperBound = cur.ub;
	int maxProfit = 0; // lower bound
	pq.push(cur);


	while (!pq.empty())
	{
		cur = pq.top();
		pq.pop();

		if (cur.ub <= maxProfit)
			continue;

		// pick (level)th item
		int level = cur.level + 1;
		node left(cur.profit + itemList[level].value, cur.weight + itemList[level].weight, level, 0);

		if (left.weight <= W)
		{
			if (left.profit > maxProfit)
				maxProfit = left.profit;
			if (level == n - 1) { // leaf
				if (left.profit == globalUpperBound)
					return left.profit;
			}
			else {
				left.ub = upperBound(left, n, W);
				if (left.ub > maxProfit)
					pq.push(left);
			}
		}

		// not pick (level)th item
		node right(cur.profit, cur.weight, level, 0);

		if (right.weight <= W)
		{
			if (right.profit > maxProfit)
				maxProfit = right.profit;
			if (level == n - 1) { // leaf
				if (right.profit == globalUpperBound)
					return right.profit;
			}
			else {
				right.ub = upperBound(right, n, W);
				if (right.ub > maxProfit)
					pq.push(right);
			}
		}
	}

	return maxProfit;
}

int main() {
	int W, n;

	while (cin >> W >> n)
	{
		itemList[1000];

		for (int i = 0; i < n; ++i) {
			int val, wei;
			cin >> val >> wei;
			item tmp(val, wei);
			itemList[i] = tmp;
		}

		struct {
			bool operator() (item a, item b) const {
				double cp1 = double(a.value) / double(a.weight);
				double cp2 = double(b.value) / double(b.weight);
				return cp1 > cp2;
			}
		} customLess;
		sort(itemList, itemList + n, customLess);

		int maxProfit = 0;
		maxProfit = KP(W, n);

		// cout << maxProfit << endl;
		printf("%d", maxProfit);

		// delete[]itemList;
	}
	return 0;
}