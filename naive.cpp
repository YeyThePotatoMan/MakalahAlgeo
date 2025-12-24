#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <chrono>

using namespace std;

typedef long double ld;
const ld INF = 1e18;

struct Plane {
    ld a, b, c;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout << fixed << setprecision(0);

    auto start = chrono::high_resolution_clock::now();

    vector<Plane> planes;
    // Reserve memory to avoid reallocation overhead
    planes.reserve(200000); 

    int Q;
    if (cin >> Q) {
        while(Q--) {
            int type;
            cin >> type;
            if (type == 1) {
                ld a, b, c;
                cin >> a >> b >> c;
                planes.push_back({a, b, c});
            } else {
                ld x, y;
                cin >> x >> y;
                
                ld ans = INF;
                bool empty = planes.empty();

                if (!empty) {
                    for (const auto& p : planes) {
                        ld val = p.a * x + p.b * y + p.c;
                        if (val < ans) ans = val;
                    }
                }

                if (empty) cout << "No planes\n";
                else cout << (long long)round(ans) << "\n";
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = end - start;
    
    cerr << "Time: " << elapsed.count() << " ms" << endl;

    return 0;
}