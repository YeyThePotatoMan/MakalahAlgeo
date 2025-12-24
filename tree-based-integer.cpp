#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long ll;
const ll INF = 4e18; 

struct Vector3 {
    ll x, y, z;
    ll dot(ll qx, ll qy) const {
        return x * qx + y * qy + z;
    }
};

struct Node {
    Vector3 plane;
    Node *child[4] = {nullptr, nullptr, nullptr, nullptr}; 
    Node(Vector3 p) : plane(p) {}
};

class LiChaoQuadTree {
private:
    Node* root = nullptr;
    ll min_x, max_x, min_y, max_y;

    ll f(Vector3 p, ll x, ll y) {
        return p.dot(x, y);
    }

    void insert(Node* &node, Vector3 p, ll lx, ll rx, ll ly, ll ry) {
        ll mx = lx + (rx - lx) / 2;
        ll my = ly + (ry - ly) / 2;

        if (!node) {
            node = new Node(p);
            return;
        }

        bool better_at_mid = f(p, mx, my) < f(node->plane, mx, my);
        if (better_at_mid) swap(node->plane, p);
        
        // CONVEXITY PRUNING (Geometri)
        bool p_useful = false;
        if (f(p, lx, ly) < f(node->plane, lx, ly)) p_useful = true;
        else if (f(p, rx, ly) < f(node->plane, rx, ly)) p_useful = true;
        else if (f(p, lx, ry) < f(node->plane, lx, ry)) p_useful = true;
        else if (f(p, rx, ry) < f(node->plane, rx, ry)) p_useful = true;

        if (!p_useful) return; 

        if (lx == rx && ly == ry) return;

        insert(node->child[0], p, lx, mx, ly, my);
        insert(node->child[1], p, mx + 1, rx, ly, my);
        insert(node->child[2], p, lx, mx, my + 1, ry);
        insert(node->child[3], p, mx + 1, rx, my + 1, ry);
    }

    ll query(Node* node, ll x, ll y, ll lx, ll rx, ll ly, ll ry) {
        if (!node) return INF;

        ll val = f(node->plane, x, y);
        
        if (lx == rx && ly == ry) return val;

        ll mx = lx + (rx - lx) / 2;
        ll my = ly + (ry - ly) / 2;

        int idx = 0;
        if (x > mx) idx |= 1; 
        if (y > my) idx |= 2; 

        ll n_lx = (idx & 1) ? mx + 1 : lx;
        ll n_rx = (idx & 1) ? rx : mx;
        ll n_ly = (idx & 2) ? my + 1 : ly;
        ll n_ry = (idx & 2) ? ry : my;

        return min(val, query(node->child[idx], x, y, n_lx, n_rx, n_ly, n_ry));
    }

public:
    LiChaoQuadTree(ll range) {
        min_x = 0; max_x = range;
        min_y = 0; max_y = range;
    }

    void add_plane(ll A, ll B, ll C) {
        Vector3 p = {A, B, C};
        insert(root, p, min_x, max_x, min_y, max_y);
    }

    ll query(ll x, ll y) {
        return query(root, x, y, min_x, max_x, min_y, max_y);
    }
};

int main() {
    ios_base::sync_with_stdio(false); 
    cin.tie(NULL);

    ll INPUT_MAX = 200000;
    ll OFFSET = 400000; 
    ll TREE_SIZE = 800000; 

    LiChaoQuadTree tree(TREE_SIZE);
    
    int Q_ops;
    if (!(cin >> Q_ops)) return 0;

    while(Q_ops--) {
        int type; 
        cin >> type;
        
        if (type == 1) {
            ll A, B, C; 
            cin >> A >> B >> C;
            
            ll A_prime = A - B;
            ll B_prime = A + B;
            ll C_prime = (2 * C) - (A_prime * OFFSET) - (B_prime * OFFSET);

            tree.add_plane(A_prime, B_prime, C_prime); 

        } else {
            ll x, y; 
            cin >> x >> y;
            
            ll u = x - y + OFFSET;
            ll v = x + y + OFFSET;
            
            // Safety Check
            if (u < 0) u = 0; if (u > TREE_SIZE) u = TREE_SIZE;
            if (v < 0) v = 0; if (v > TREE_SIZE) v = TREE_SIZE;
            
            ll ans = tree.query(u, v);
            
            if (ans >= INF/2) cout << "No planes\n";
            else {
                cout << ans / 2 << "\n";
            }
        }
    }
    return 0;
}