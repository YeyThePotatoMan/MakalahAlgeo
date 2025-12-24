#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <random>
#include <chrono>

using namespace std;

typedef long double ld;
const ld INF = 1e18; 
const ld EPS = 1e-9;

ld CT, ST; 

struct Plane {
    ld x, y, z;
    ld eval(ld qx, ld qy) const {
        return x * qx + y * qy + z;
    }
};

pair<ld, ld> rotate_pt(ld x, ld y) {
    return {x * CT - y * ST, x * ST + y * CT};
}

struct Node {
    Plane p;
    Node *ch[4]; 
    Node(Plane _p) : p(_p) {
        for(int i=0; i<4; i++) ch[i] = nullptr;
    }
};

class LiChao {
    Node* root;
    ld min_v, max_v;

    ld f(Plane p, ld x, ld y) {
        return p.eval(x, y);
    }

    void insert(Node* &n, Plane p, ld lx, ld rx, ld ly, ld ry) {
        ld mx = (lx + rx) / 2.0;
        ld my = (ly + ry) / 2.0;

        if (!n) {
            n = new Node(p);
            return;
        }

        bool better = f(p, mx, my) < f(n->p, mx, my);
        if (better) swap(n->p, p);

        int wins = 0;
        if (f(p, lx, ly) < f(n->p, lx, ly) - EPS) wins++;
        else if (f(p, rx, ly) < f(n->p, rx, ly) - EPS) wins++;
        else if (f(p, lx, ry) < f(n->p, lx, ry) - EPS) wins++;
        else if (f(p, rx, ry) < f(n->p, rx, ry) - EPS) wins++;

        if (wins == 0) return; 
        if ((rx - lx) < 0.5 && (ry - ly) < 0.5) return;

        insert(n->ch[0], p, lx, mx, ly, my);
        insert(n->ch[1], p, mx, rx, ly, my);
        insert(n->ch[2], p, lx, mx, my, ry);
        insert(n->ch[3], p, mx, rx, my, ry);
    }

    ld query(Node* n, ld x, ld y, ld lx, ld rx, ld ly, ld ry) {
        if (!n) return INF;

        ld val = f(n->p, x, y);
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
    LiChao(ld sz) : root(nullptr), min_v(0), max_v(sz) {}

    void add(ld A, ld B, ld C, ld off) {
        // Rotate normal vector
        pair<ld, ld> r = rotate_pt(A, B);
        // Adjust C for offset
        ld C_prime = C - (r.first * off) - (r.second * off);
        insert(root, {r.first, r.second, C_prime}, min_v, max_v, min_v, max_v);
    }

    ld get(ld x, ld y, ld off) {
        // Rotate query point
        pair<ld, ld> r = rotate_pt(x, y);
        // Apply offset and clamp
        ld u = r.first + off;
        ld v = r.second + off;
        if (u < min_v) u = min_v; if (u > max_v) u = max_v;
        if (v < min_v) v = min_v; if (v > max_v) v = max_v;
        return query(root, u, v, min_v, max_v, min_v, max_v);
    }
};

int main() {
    ios_base::sync_with_stdio(false); 
    cin.tie(NULL);
    cout << fixed << setprecision(0);

    
    // Random rotation setup
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<ld> dis(0.2, 1.0); 
    ld theta = dis(gen); 
    CT = cos(theta);
    ST = sin(theta);
    
    auto start_time = chrono::high_resolution_clock::now();
    
    ld OFFSET = 400000.0; 
    ld SZ = 800000.0; 

    LiChao tree(SZ);
    
    int Q;
    if (cin >> Q) {
        while(Q--) {
            int type; 
            cin >> type;
            if (type == 1) {
                ld A, B, C; 
                cin >> A >> B >> C;
                tree.add(A, B, C, OFFSET);
            } else {
                ld x, y; 
                cin >> x >> y;
                ld ans = tree.get(x, y, OFFSET);
                if (ans >= INF/2.0) cout << "No planes\n";
                else cout << (long long)round(ans) << "\n";
            }
        }
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = end_time - start_time;
    
    cerr << "Time: " << elapsed.count() << " ms" << endl;

    return 0;
}