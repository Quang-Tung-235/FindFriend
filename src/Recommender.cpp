#include "Recommender.hpp"
#include <queue>
#include <unordered_set>
using namespace std;

// Hàm tính chỉ số Jaccard giữa 2 tập (tập sở thích hoặc thói quen)
static double jaccard(const unordered_set<int>& a, const unordered_set<int>& b) {
    if (a.empty() && b.empty()) return 0.0;  // Nếu cả 2 tập rỗng thì tương đồng 0
    size_t inter = 0;
    for (int x : a)
        if (b.count(x)) ++inter;            // Đếm số phần tử chung
    size_t uni = a.size() + b.size() - inter;  // Tính phần hợp của 2 tập
    return double(inter) / uni;               // Công thức Jaccard: |A ∩ B| / |A ∪ B|
}

// Hàm gợi ý bạn bè cho uid
vector<Recommender::Candidate> Recommender::suggest(const string& uid, size_t k, double wM, double wJ) const {
    auto uOpt = g_.get(uid);
    if (!uOpt) return {};   // Nếu không có uid trong đồ thị thì trả về rỗng
    const Student& u = **uOpt;

    unordered_set<string> seen = u.friends;  // Đã biết các bạn của u
    seen.insert(uid);                         // Và cả chính u để tránh gợi ý bản thân

    // Định nghĩa hàm so sánh cho priority_queue (max-heap theo điểm score)
    struct Cmp {
        bool operator()(const Candidate& a, const Candidate& b) {
            return a.score < b.score;
        }
    };

    priority_queue<Candidate, vector<Candidate>, Cmp> pq;

    // Duyệt bạn của u (độ sâu 1)
    for (const auto& fId : u.friends) {
        auto fOpt = g_.get(fId);
        if (!fOpt) continue;
        const Student& f = **fOpt;

        // Duyệt bạn của bạn (độ sâu 2)
        for (const auto& fof : f.friends) {
            if (seen.count(fof)) continue;  // Bỏ qua nếu đã có trong seen
            auto cOpt = g_.get(fof);
            if (!cOpt) continue;
            const Student& cand = **cOpt;

            // Tính số lượng bạn chung (mutual friends)
            double mutual = 0;
            for (auto& x : cand.friends)
                if (u.friends.count(x)) ++mutual;

            // Tính độ tương đồng Jaccard trên sở thích và thói quen
            double sim = jaccard(u.hobbies, cand.hobbies) + jaccard(u.habits, cand.habits);

            // Tính điểm score theo trọng số wM (mutual) và wJ (Jaccard)
            double score = wM * mutual + wJ * sim;

            pq.push({ cand.id, score });  // Đẩy vào max-heap
            seen.insert(fof);             // Đánh dấu đã xét
        }
    }

    // Lấy top-k ứng viên có điểm cao nhất
    vector<Candidate> res;
    while (!pq.empty() && res.size() < k) {
        res.push_back(pq.top());
        pq.pop();
    }

    return res;
}
