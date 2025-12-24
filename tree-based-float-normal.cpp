#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <chrono>

using namespace std;

typedef long double ld;
const ld INF = 1e18;
const ld EPS = 1e-9;

struct Plane {
    ld a, b, c;
    ld eval(ld x, ld y) const {
        return a * x + b * y + c;
    }
};

struct Node {
    Plane p;
    Node *ch[4]; 
    Node(Plane _p) : p(_p) {
        for(int i=0; i<4; i++) ch[i] = nullptr;
    }
};

class LiChao {
    Node* root;
    ld min_c, max_c;

    void insert(Node* &n, Plane p, ld lx, ld rx, ld ly, ld ry) {
        ld mx = (lx + rx) / 2.0;
        ld my = (ly + ry) / 2.0;

        if (!n) {
            n = new Node(p);
            return;
        }

        bool better = p.eval(mx, my) < n->p.eval(mx, my);
        if (better) swap(n->p, p);

        // Pruning
        int wins = 0;
        if (p.eval(lx, ly) < n->p.eval(lx, ly) - EPS) wins++;
        else if (p.eval(rx, ly) < n->p.eval(rx, ly) - EPS) wins++;
        else if (p.eval(lx, ry) < n->p.eval(lx, ry) - EPS) wins++;
        else if (p.eval(rx, ry) < n->p.eval(rx, ry) - EPS) wins++;

        if (wins == 0) return; 
        if ((rx - lx) < 0.5 && (ry - ly) < 0.5) return;

        insert(n->ch[0], p, lx, mx, ly, my);
        insert(n->ch[1], p, mx, rx, ly, my);
        insert(n->ch[2], p, lx, mx, my, ry);
        insert(n->ch[3], p, mx, rx, my, ry);
    }

    ld query(Node* n, ld x, ld y, ld lx, ld rx, ld ly, ld ry) {
        if (!n) return INF;
        
        ld val = n->p.eval(x, y);
        if ((rx - lx) < 0.5 && (ry - ly) < 0.5) return val;

        ld mx = (lx + rx) / 2.0;
        ld my = (ly + ry) / 2.0;

        int idx = 0;
        if (x > mx) idx |= 1;
        if (y > my) idx |= 2;

        ld nlx = (idx&1) ? mx : lx;
        ld nrx = (idx&1) ? rx : mx;
        ld nly = (idx&2) ? my : ly;
        ld nry = (idx&2) ? ry : my;

        return min(val, query(n->ch[idx], x, y, nlx, nrx, nly, nry));
    }

public:
    LiChao(ld sz) : root(nullptr), min_c(0), max_c(sz) {}

    void add(ld a, ld b, ld c) {
        insert(root, {a, b, c}, min_c, max_c, min_c, max_c);
    }

    ld get(ld x, ld y) {
        return query(root, x, y, min_c, max_c, min_c, max_c);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout << fixed << setprecision(0);

    auto start = chrono::high_resolution_clock::now();

    // Standard bounds, no offset needed for non-rotated
    LiChao tree(200000.0);

    int Q;
    if (cin >> Q) {
        while(Q--) {
            int type;
            cin >> type;
            if (type == 1) {
                ld a, b, c;
                cin >> a >> b >> c;
                tree.add(a, b, c);
            } else {
                ld x, y;
                cin >> x >> y;
                ld ans = tree.get(x, y);
                if (ans >= INF/2.0) cout << "No planes\n";
                else cout << (long long)round(ans) << "\n";
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = end - start;
    
    // Output time to stderr
    cerr << "Time: " << elapsed.count() << " ms" << endl;

    return 0;
}