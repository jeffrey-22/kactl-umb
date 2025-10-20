#include "../utilities/template.h"
#include "../../content/data-structures/SegTreeBeats.h"

int main() {
    SegTreeBeats T = SegTreeBeats();
    T.update_add(1, 10, 2);
    T.update_chmax(1, 5, 3);
    T.update_chmin(5, 6, 0);
    T.update_chmin(0, 3, 4);
    T.update_chmin(7, 8, 4);
    // 3 3 3 3 0 0 2 2 2 2
    assert(T.query_sum(1, 10) == 20);
    assert(T.query_sum(1, 1) == 3);
    assert(T.query_sum(4, 5) == 3);
	cout<<"Tests passed!"<<endl;
}