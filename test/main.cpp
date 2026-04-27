#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TIME_SLOTS 10
#define NUM_HOSTS  3

// ────────────────────────────────────────────────
//  Host: represents a device competing for the bus
// ────────────────────────────────────────────────
class Host {
    int  id;
    bool pending = false;
public:
    int  priority;
    int  granted_count = 0;  // total times granted access
    int  wait_count    = 0;  // total times denied while requesting

    Host(int id, int priority) : id(id), priority(priority) {}

    int  getID()           { return id; }
    bool hasRequest()      { return pending; }
    void setRequest(bool r){ pending = r; }
    void setPriority(int p){ priority = p; }
};

// ────────────────────────────────────────────────
//  Print the request schedule before simulation
// ────────────────────────────────────────────────
void printSchedule(bool requests[NUM_HOSTS][TIME_SLOTS]) {
    printf("=== Request Schedule ===\n");
    printf("%-6s", "Slot");
    for (int h = 0; h < NUM_HOSTS; h++)
        printf("  Host%d", h + 1);
    printf("\n");
    for (int t = 0; t < TIME_SLOTS; t++) {
        printf("%-6d", t);
        for (int h = 0; h < NUM_HOSTS; h++)
            printf("  %-5s", requests[h][t] ? "REQ" : "-");
        printf("\n");
    }
    printf("\n");
}

// ────────────────────────────────────────────────
//  Print final statistics
// ────────────────────────────────────────────────
void printStats(Host hosts[], int n) {
    printf("\n=== Statistics ===\n");
    printf("%-8s  %-10s  %-10s\n", "Host", "Granted", "Denied");
    for (int i = 0; i < n; i++)
        printf("%-8d  %-10d  %-10d\n",
               hosts[i].getID(),
               hosts[i].granted_count,
               hosts[i].wait_count);
}

// ────────────────────────────────────────────────
//  Algorithm 1: Priority-based arbitration
//  The host with the highest priority gets access.
//  After being granted, its priority decreases by 1
//  to prevent starvation. Resets when all go below 0.
// ────────────────────────────────────────────────
void runPriorityArbiter(Host hosts[], bool requests[NUM_HOSTS][TIME_SLOTS]) {
    printf("=== Algorithm: Priority-Based ===\n\n");

    for (int t = 0; t < TIME_SLOTS; t++) {
        // Reset priorities if all are negative
        bool all_negative = true;
        for (int i = 0; i < NUM_HOSTS; i++)
            if (hosts[i].priority >= 0) { all_negative = false; break; }
        if (all_negative)
            for (int i = 0; i < NUM_HOSTS; i++)
                hosts[i].setPriority(0);

        // Load requests for this slot
        for (int i = 0; i < NUM_HOSTS; i++)
            if (requests[i][t])
                hosts[i].setRequest(true);

        // Find highest-priority host with a pending request
        int winner = -1;
        for (int i = 0; i < NUM_HOSTS; i++) {
            if (!hosts[i].hasRequest()) continue;
            if (winner == -1 || hosts[i].priority > hosts[winner].priority)
                winner = i;
        }

        printf("Slot %d: ", t);
        if (winner == -1) {
            printf("no requests\n");
        } else {
            printf("Host%d granted (priority %d)\n",
                   hosts[winner].getID(), hosts[winner].priority);
            hosts[winner].setRequest(false);
            hosts[winner].priority--;
            hosts[winner].granted_count++;

            // Denied hosts accumulate wait count
            for (int i = 0; i < NUM_HOSTS; i++)
                if (hosts[i].hasRequest()) hosts[i].wait_count++;
        }
    }
}

// ────────────────────────────────────────────────
//  Algorithm 2: Round Robin
//  Each slot, rotate to the next host in order.
//  Skips hosts without a pending request.
// ────────────────────────────────────────────────
void runRoundRobin(Host hosts[], bool requests[NUM_HOSTS][TIME_SLOTS]) {
    printf("=== Algorithm: Round Robin ===\n\n");

    int turn = 0;  // which host gets priority this slot

    for (int t = 0; t < TIME_SLOTS; t++) {
        for (int i = 0; i < NUM_HOSTS; i++)
            if (requests[i][t])
                hosts[i].setRequest(true);

        // Find the next requesting host starting from `turn`
        int winner = -1;
        for (int i = 0; i < NUM_HOSTS; i++) {
            int idx = (turn + i) % NUM_HOSTS;
            if (hosts[idx].hasRequest()) { winner = idx; break; }
        }

        printf("Slot %d: ", t);
        if (winner == -1) {
            printf("no requests\n");
        } else {
            printf("Host%d granted\n", hosts[winner].getID());
            hosts[winner].setRequest(false);
            hosts[winner].granted_count++;
            for (int i = 0; i < NUM_HOSTS; i++)
                if (hosts[i].hasRequest()) hosts[i].wait_count++;
            turn = (winner + 1) % NUM_HOSTS;
        }
    }
}

// ────────────────────────────────────────────────
//  Algorithm 3: First Come First Served (FCFS)
//  The host that sent its request earliest gets access.
// ────────────────────────────────────────────────
void runFCFS(Host hosts[], bool requests[NUM_HOSTS][TIME_SLOTS]) {
    printf("=== Algorithm: First Come First Served ===\n\n");

    int arrival[NUM_HOSTS];
    memset(arrival, -1, sizeof(arrival));  // -1 = not waiting

    for (int t = 0; t < TIME_SLOTS; t++) {
        // Record arrival time for new requests
        for (int i = 0; i < NUM_HOSTS; i++)
            if (requests[i][t] && arrival[i] == -1)
                arrival[i] = t;

        // Find the host that arrived earliest
        int winner = -1;
        for (int i = 0; i < NUM_HOSTS; i++) {
            if (arrival[i] == -1) continue;
            if (winner == -1 || arrival[i] < arrival[winner])
                winner = i;
        }

        printf("Slot %d: ", t);
        if (winner == -1) {
            printf("no requests\n");
        } else {
            printf("Host%d granted (arrived at slot %d)\n",
                   hosts[winner].getID(), arrival[winner]);
            arrival[winner] = -1;
            hosts[winner].granted_count++;
            for (int i = 0; i < NUM_HOSTS; i++)
                if (arrival[i] != -1) hosts[i].wait_count++;
        }
    }
}

// ────────────────────────────────────────────────
//  Helper: reset host state between algorithm runs
// ────────────────────────────────────────────────
void resetHosts(Host hosts[], int n) {
    for (int i = 0; i < n; i++) {
        hosts[i].setRequest(false);
        hosts[i].setPriority(0);
        hosts[i].granted_count = 0;
        hosts[i].wait_count    = 0;
    }
}

// ────────────────────────────────────────────────
//  Main
// ────────────────────────────────────────────────
int main() {
    srand((unsigned)time(NULL));

    // Generate a random request schedule shared across all algorithms
    bool requests[NUM_HOSTS][TIME_SLOTS];
    for (int h = 0; h < NUM_HOSTS; h++)
        for (int t = 0; t < TIME_SLOTS; t++)
            requests[h][t] = rand() % 2;

    printSchedule(requests);

    Host hosts[NUM_HOSTS] = { Host(1, 0), Host(2, 0), Host(3, 0) };

    // Run all three algorithms on the same request schedule
    runPriorityArbiter(hosts, requests);
    printStats(hosts, NUM_HOSTS);

    printf("\n");
    resetHosts(hosts, NUM_HOSTS);

    runRoundRobin(hosts, requests);
    printStats(hosts, NUM_HOSTS);

    printf("\n");
    resetHosts(hosts, NUM_HOSTS);

    runFCFS(hosts, requests);
    printStats(hosts, NUM_HOSTS);

    return 0;
}
