#include <fstream>
#include <vector>
#include "graphics.hpp"
#include <iostream>
#include <list>
#include <map>
#include <queue>

using namespace genv;
using namespace std;

struct Koord {
    int x,y;
    Koord(int x, int y): x(x), y(y){};

    bool operator<(const Koord& other) const {
        if (x != other.x) {
            return x<other.x;
        }
        return y<other.y;
    }

    bool operator==(const Koord& other) const {
        return (x == other.x && y == other.y);
    }

    Koord& operator=(const Koord& other) {
        x = other.x;
        y = other.y;

        return *this;
    }
};

class VisualMap {
    int X, Y;
    vector<vector<int>> m;
    int r =2;
    const int outter_corner_rad=100;

    canvas img;

    Koord last_cursor_pos{0,0};

    Koord point_A{0,0};
    bool has_A=false;
    bool has_B=false;
    int click_count=0;

    Koord point_B{0,0};

    map<Koord, int> dist_map{};
    list<Koord>curr_origin_list{};

    vector<Koord> final_path{};

    bool search_finished = false;
    int final_step_count = 0;

    void search_shortest_distance() {
        if (!has_A || !has_B) return;

        dist_map.clear();
        curr_origin_list.clear();
        final_path.clear();
        bool AB_connected=false;

        curr_origin_list.emplace_back(point_A);
        dist_map[point_A]=0;

        while (!curr_origin_list.empty()) {
            Koord curr = curr_origin_list.front();
            curr_origin_list.pop_front();

            if (curr == point_B) {
                AB_connected=true;
                break;
            }

            const int dx[] = {-1, 1, 0, 0};
            const int dy[] = {0, 0, -1, 1};

            for (int d=0; d<4; d++) {
                int i = curr.x + dx[d];
                int j = curr.y + dy[d];

                if (j>=0 && j < Y && i>=0 && i <X) {
                    Koord next_co(i,j);

                    if (dist_map.find(next_co)== dist_map.end() && m[j][i]>0) {
                        dist_map[next_co] = dist_map[curr] + 1;
                        curr_origin_list.emplace_back(next_co);
                    }
                }
            }
        }
        if (AB_connected) {
            Koord curr = point_B;
            final_step_count = 0;
            while (dist_map[curr]!=0) {

                const int dx[] = {0, 0, -1, 1};
                const int dy[] = {-1, 1, 0, 0};

                for (int d=0; d<4; d++) {
                    int i = curr.x + dx[d];
                    int j = curr.y + dy[d];

                    if (j>=0 && j < Y && i>=0 && i <X) {
                        Koord next_co(i,j);

                        if (dist_map.find(next_co)!=dist_map.end() && dist_map[curr]-1==dist_map[next_co]) {
                            final_path.emplace_back(next_co);
                            curr = next_co;
                            final_step_count++;
                            break;
                        }
                    }
                }
            }
            cout<<"To reach from point A to point B, it took the algorythm "<<final_step_count<<" steps. :)"<<endl;
        }
        else {
            final_step_count = -1;
            cout<<"The point could not have been connected!"<<endl;
        }
        search_finished = true;

    }

    void search_shortest_distance2() { // DIAGONAL VERSION + Dijkstra's Algorithm
        if (!has_A || !has_B) return;

        dist_map.clear();
        final_path.clear();
        bool AB_connected=false;

        priority_queue<pair<int, Koord>, vector<pair<int, Koord>>, greater<pair<int, Koord>>> pq;

        pq.push({0, point_A});
        dist_map[point_A]=0;

        while (!pq.empty()) {
            int curr_dist = pq.top().first;
            Koord curr = pq.top().second;
            pq.pop();

            if (curr_dist > dist_map[curr]) continue; //if found better path

            if (curr == point_B) {
                AB_connected=true;
                break;
            }

            const int dx[] = {-1, 1, 0, 0,  -1, 1, -1, 1};
            const int dy[] = {0, 0, -1, 1,  -1, -1, 1, 1};

            for (int d=0; d<8; d++) {
                int i = curr.x + dx[d];
                int j = curr.y + dy[d];

                if (j>=0 && j < Y && i>=0 && i <X) {
                    Koord next_co(i,j);

                    int step_weight = (d<4) ?10 : 14;

                    if (m[j][i] > 0) {
                        // Make sure it's land
                        int new_dist = dist_map[curr] + step_weight;

                        if (dist_map.find(next_co) == dist_map.end() || new_dist < dist_map[next_co]) {
                            dist_map[next_co] = new_dist;
                            pq.push({new_dist, next_co});
                        }
                    }
                }
            }
        }
        if (AB_connected) {
            Koord curr = point_B;
            final_step_count = 0;
            while (dist_map[curr]!=0) {

                const int dx[] = {-1, 1, 0, 0,  -1, 1, -1, 1};
                const int dy[] = {0, 0, -1, 1,  -1, -1, 1, 1};

                for (int d=0; d<8; d++) {
                    int i = curr.x + dx[d];
                    int j = curr.y + dy[d];

                    if (j>=0 && j < Y && i>=0 && i <X) {
                        Koord next_co(i,j);

                        int step_weight = (d<4) ? 10 : 14;

                        if (dist_map.find(next_co) != dist_map.end() &&
                            dist_map[next_co] + step_weight == dist_map[curr]) {
                            final_path.emplace_back(next_co);
                            curr = next_co;
                            final_step_count++;
                            break;
                        }
                    }
                }
            }
            cout<<"To reach from point A to point B, it took the algorythm "<<final_step_count<<" steps. :)"<<endl;
        }
        else {
            final_step_count = -1;
            cout<<"The point could not have been connected!"<<endl;
        }
        search_finished = true;

    }


    //vector<vector<bool>> infected;

   //list<Koord> to_infect;
/*
    void reset_infection() {
        infected= vector<vector<bool>>(Y, vector<bool>(X, false));
    }

    void add_infection(int x, int y) {
        if (x >= 0 && x < X && y >= 0 && y < Y) {
            if (!infected[y][x] && m[y][x] > 0) {
                infected[y][x] = true;
                to_infect.emplace_back(x, y);
            }
        }
    }

    void spread_infection() {
        list<Koord> next_wave;

        for (auto &k : to_infect) {
            const int dx[] = {1, -1, 0, 0};
            const int dy[] = {0, 0, 1, -1};

            for (int d = 0; d < 4; d++) {
                int i = k.x + dx[d];
                int j = k.y + dy[d];

                    if (j >= 0 && j < Y && i >= 0 && i < X) {

                        // skip self
                        if (i == k.x && j == k.y) continue;


                        if (!infected[j][i] && m[j][i] > 0) {
                            infected[j][i] = true;
                            next_wave.emplace_back(i, j);
                        }
                    }

            }
        }

        to_infect = next_wave;
    }
    */

    void create_canvas() {
        img.open(X*r,Y*r);
        img<<move_to(0,0);
        for (int i = 0; i < Y; i++) {
            for (int j = 0; j < X; j++) {

                int h = m[i][j];

                if (h <= 0) {
                    int depth = min(abs(h) * 8, 120);

                    int r_col = 50;
                    int g_col = 120 + (120 - depth) / 4;
                    int b_col = 255 - depth;

                    img << move_to(j*r, i*r)
                         << color(r_col, g_col, b_col)
                         << box(r, r);

                } else {
                    int g_col = min(120 + h * 5, 200);

                    img << move_to(j*r, i*r)
                         << color(40, g_col, 40)
                         << box(r, r);

                }
            }
        }
    }

    void show_map() {
        gout<<stamp(img,0,0);
        //highlights path of search
        if (search_finished) {
            for (auto k: dist_map) {
                gout<<move_to(k.first.x*r, k.first.y*r)<<color(133, 133, 133)<<box(r,r);
            }
            for (auto p : final_path) {
                gout<<move_to(p.x*r, p.y*r)<<color(255, rand()%184, 28)<<box(r,r);
            }
        }
    }

    void show_mouse_pos(int mx, int my) {
        gout<<color(255,0,0)
            <<move_to(min(X*r,mx),min(my,Y*r))
            <<line_to(0,min(my,Y*r))
            <<line_to(X*r,min(Y*r,my));

        gout<<color(255,0,0)
            <<move_to(min(X*r,mx),min(my,Y*r))
            <<line_to(min(X*r,mx),0)
            <<line_to(min(X*r,mx),Y*r);
    }

    void show_bottom_panel(int my) {
        int row = my / r;
        row = max(0, min(Y - 1, row));

        for (int i = 0; i < X; i++) {
            int h = m[row][i];

            gout << move_to(i * r, Y * r + outter_corner_rad/2);

            if (h>=0){gout<< color(50, 240, 50);}
            else {gout<< color(50, 50, 240);}

            gout<< box(r, -h/2);
        }
    }

    void show_right_panel(int mx) {
        int col = mx / r;
        col = max(0, min(X - 1, col));

        for (int i = 0; i < Y; i++) {
            int h = m[i][col];

            gout << move_to(X * r + outter_corner_rad/2, i * r);

            if (h>=0){gout<< color(50, 240, 50);}
            else {gout<< color(50, 50, 240);}

            gout<< box(h/2, r);
        }
    }

    void display() {
        gout<<color(0,0,0)
            <<move_to(0,0)
            <<box(X*r+outter_corner_rad,Y*r+outter_corner_rad);

        show_map();
        if (has_A) {
            gout << move_to(point_A.x*r, point_A.y*r)<<color(max(rand()%235,180), 20, 15)<<box(r*r*r, r*r);
        }
        if (has_B) {
            gout << move_to(point_B.x*r, point_B.y*r)<<color(10, 80, max(rand()%240,100))<<box(r*r*r, r*r);
        }
        show_status_box();
    }

    void display_cursor(int mx, int my) {
        show_mouse_pos(mx, my);

        if (mx < X*r && my < Y*r) {
            show_bottom_panel(my);
            show_right_panel(mx);
        }
    }

    void show_status_box() {
        gout << move_to(X * r, Y * r) << color(30, 30, 30) << box(outter_corner_rad, outter_corner_rad);

        if (search_finished) {
            if (final_step_count != -1) {
                gout << color(220, 220, 220);
                gout << move_to(X * r + 5, Y * r + 35) << text("Path Found!");

                gout << color(255, 184, 28);
                gout << move_to(X * r + 5, Y * r + 60) << text("Steps: " + to_string(final_step_count));
            } else {
                gout << color(255, 80, 80);
                gout << move_to(X * r + 5, Y * r + 45) << text("UNREACHABLE!");
            }
        }
    }

public:
    VisualMap(string filename) {
        fstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open file.\n";
            exit(12);
        }
        file>>X>>Y;
        for (int i=0; i<Y;i++) {
            vector<int> v;
            for (int j =0; j<X;j++) {
                int pixel;
                file>>pixel;
                v.emplace_back(pixel);
            }
            m.emplace_back(v);
        }
        file.close();
        //reset_infection();
    }

    void run() {
        gout.open(X*r+outter_corner_rad, Y*r+outter_corner_rad);
        event ev;
        gin.timer(50);
        create_canvas();

        //show_map();
        gout << refresh;

        while(gin >> ev){
            if (ev.button==btn_left) {
                if (ev.pos_x >= X * r || ev.pos_y >= Y * r || m[ev.pos_y/r][ev.pos_x/r] <=0){ continue;}

                click_count++;
                if (click_count %2 ==1) {
                    point_A = {ev.pos_x/ r, ev.pos_y/ r};
                    has_A =true;
                    has_B =false;
                    search_finished = false;
                }
                else {
                    point_B = {ev.pos_x/ r, ev.pos_y/ r};
                    has_B=true;
                    search_shortest_distance2();
                }
                //add_infection(ev.pos_x/ r, ev.pos_y/ r);
            }
            else if (ev.type == ev_timer) {
                //spread_infection();
                display();
                display_cursor(last_cursor_pos.x, last_cursor_pos.y);

            }
            else if (ev.keycode==key_space) {
                //reset_infection();
            }

            if (ev.type == ev_mouse){
                last_cursor_pos.x =ev.pos_x;
                last_cursor_pos.y =ev.pos_y;

            }
            gout << refresh;
        }

    }
};

int main() {
    VisualMap terkep("terkep.txt");
    terkep.run();
    return 0;
}
