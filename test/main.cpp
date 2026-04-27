#define ts 10 //time slot
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
class host{
    int the_ID;
    bool the_request = false;
public:
    int the_priority;
    host(int id, int p): the_ID(id), the_priority(p) {}
    int ID() {
        return the_ID;
    }
    void request(bool request){
        the_request = request;
    }
    void priority(int p) {
        the_priority = p;
    }
    bool request(){
        return the_request;
    }
    int priority(){
        return the_priority;
    }
};
void arbiter(host& a, host& b, host& c);
void print_status(bool *host1_r, bool *host2_r, bool *host3_r);

int main()
{
    srand((unsigned)time(NULL));
    host a(1, 0);
    host b(2, 0);
    host c(3, 0);
    bool host1_r[ts];
    bool host2_r[ts];
    bool host3_r[ts];
    for (int i = 0; i < ts; i++) {
        host1_r[i] = (rand()%2);
        host2_r[i] = (rand()%2);
        host3_r[i] = (rand()%2);
    }
    print_status(host1_r, host2_r, host3_r);
    printf("-------\n");
    for (int t = 0; t < ts; t++) {
        if (a.priority() < 0 && b.priority() < 0 && c.priority() < 0) {
            a.priority(0); b.priority(0); c.priority(0);
        }
        if (host1_r[t])
            a.request(1);
        if (host2_r[t])
            b.request(1);
        if (host3_r[t])
            c.request(1);
        printf("\n%d before arbiter : %d, %d, %d.\n", t, a.request(), b.request(), c.request());
        printf("%d before priority : %d, %d, %d.\n", t, a.priority(), b.priority(), c.priority());
        if (a.priority() >= b.priority() && b.priority() >= c.priority()) {
            arbiter(a, b, c);
        } else if (a.priority() >= c.priority() && c.priority() >= b.priority()) {
            arbiter(a, c, b);
        } else if (b.priority() >= a.priority() && a.priority() >= c.priority()) {
            arbiter(b, a, c);
        } else if (b.priority() >= c.priority() && c.priority() >= a.priority()) {
            arbiter(b, c, a);
        } else if (c.priority() >= a.priority() && a.priority() >= b.priority()) {
            arbiter(c, a, b);
        } else if (c.priority() >= b.priority() && b.priority() >= a.priority()) {
            arbiter(c, b, a);
        }
        printf("\n%d after arbiter : %d, %d, %d.\n", t, a.request(), b.request(), c.request());
        printf("%d after priority : %d, %d, %d.\n", t, a.priority(), b.priority(), c.priority());
    }
    return 0;
}
void arbiter(host& a, host& b, host& c) {
    
    if(a.request() != 0) {
            a.request(0);
            a.the_priority --;
            printf("Arbiter Result: host %d \n", a.ID());
        } else if(b.request() != 0) {
            b.request(0);
            b.the_priority --;
            printf("Arbiter Result: host %d \n", b.ID());
        } else {
            c.request(0);
            c.the_priority --;
            printf("Arbiter Result: host %d \n", c.ID());
        }
}
void print_status(bool *host1_r, bool *host2_r, bool *host3_r) {
    for (int i = 0; i < ts; i++) {
        printf("%d, request : %d, %d, %d.\n", i, host1_r[i], host2_r[i], host3_r[i]);
    }
}



