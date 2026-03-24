#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

#define MAX_NAME_LENGTH 50
#define MAX_DESTINATIONS 10
#define NUM_SUGGESTED 8
#define MAX_HALTS 8

// Pre-defined suggested destinations (India only)
struct SuggestedDestination {
    char name[MAX_NAME_LENGTH];
    int recommended_days;
    int estimated_cost;
    int distance_to_next; // distance to next destination in the list (km)
};

static struct SuggestedDestination suggestedList[NUM_SUGGESTED] = {
    {"Delhi",       4, 15000, 200},
    {"Agra",        2,  8000, 240},
    {"Jaipur",      3, 10000, 1150},
    {"Mumbai",      4, 18000, 590},
    {"Goa",         5, 12000, 650},
    {"Kochi",       3, 11000, 540},
    {"Bangalore",   3, 13000, 350},
    {"Chennai",     3, 10000, 0}
};

void showSuggestions() {
    printf("\n  Available Suggested Destinations (India):\n");
    printf("  %-4s %-15s %-10s %-14s\n", "No.", "Destination", "Days", "Est. Cost(Rs.)");
    printf("  -------------------------------------------\n");
    for (int i = 0; i < NUM_SUGGESTED; i++) {
        printf("  [%d] %-15s %-10d Rs.%-11d\n",
               i + 1,
               suggestedList[i].name,
               suggestedList[i].recommended_days,
               suggestedList[i].estimated_cost);
    }
    printf("  [0] Enter custom destination (India only)\n");
}

// Structure to represent a tree node
struct TreeNode {
    char name[MAX_NAME_LENGTH];
    int days;
    int cost;
    struct TreeNode* activities;
    struct TreeNode* next;
};

// Structure to represent a destination and its distance
struct Destination {
    char name[MAX_NAME_LENGTH];
    int distance;
};

// Function to add a node to the tree and update remaining days and cost
void addNode(struct TreeNode* parent, const char* name, int days, int cost, int* remainingDays, int* remainingCost) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    strncpy(newNode->name, name, sizeof(newNode->name) - 1);
    newNode->name[sizeof(newNode->name) - 1] = '\0';
    newNode->days = days;
    newNode->cost = cost;
    newNode->activities = NULL;
    newNode->next = NULL;

    if (parent->activities == NULL) {
        parent->activities = newNode;
    } else {
        struct TreeNode* current = parent->activities;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }

    // Update remaining days and cost
    *remainingDays -= days;
    *remainingCost -= cost;
}

// Function to display only activities in the tree structure
void displayActivities(struct TreeNode* root) {
    if (root == NULL) {
        return;
    }

    // Recursively display sub-activities
    struct TreeNode* current = root->activities;
    while (current != NULL) {
        printf("Activity: %s (Days: %d, Cost: Rs.%d)\n", current->name, current->days, current->cost);
        displayActivities(current);
        current = current->next;
    }
}

// Find a destination index by name (case-insensitive). Returns -1 if not found.
static int findDestByName(struct Destination dests[], int num, const char* name) {
    for (int i = 0; i < num; i++) {
        int match = 1;
        int j = 0;
        while (dests[i].name[j] != '\0' || name[j] != '\0') {
            if (tolower((unsigned char)dests[i].name[j]) != tolower((unsigned char)name[j])) {
                match = 0;
                break;
            }
            j++;
        }
        if (match) return i;
    }
    return -1;
}

// Core Dijkstra on a 2-D adjacency matrix.
// Fills path[] with node indices (source first), sets *pathLen.
// Returns total distance, or INT_MAX if unreachable.
static int runDijkstra(int dist2d[MAX_DESTINATIONS][MAX_DESTINATIONS], int num,
                       int src, int dst,
                       int path[MAX_DESTINATIONS], int* pathLen) {
    int dist[MAX_DESTINATIONS];
    int visited[MAX_DESTINATIONS];
    int prev[MAX_DESTINATIONS];

    for (int i = 0; i < num; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
        prev[i] = -1;
    }
    dist[src] = 0;

    for (int count = 0; count < num - 1; count++) {
        // Pick unvisited node with smallest tentative distance
        int u = -1;
        int minDist = INT_MAX;
        for (int v = 0; v < num; v++) {
            if (!visited[v] && dist[v] < minDist) {
                u = v;
                minDist = dist[v];
            }
        }
        // All remaining nodes unreachable
        if (u == -1) break;
        visited[u] = 1;

        for (int v = 0; v < num; v++) {
            // Guard: skip disconnected edges and prevent INT_MAX overflow
            if (!visited[v] && dist2d[u][v] != INT_MAX && dist[u] != INT_MAX &&
                dist2d[u][v] + dist[u] < dist[v]) {
                dist[v] = dist2d[u][v] + dist[u];
                prev[v] = u;
            }
        }
    }

    if (dist[dst] == INT_MAX) {
        *pathLen = 0;
        return INT_MAX;
    }

    // Reconstruct path by tracing prev[] from dst back to src
    *pathLen = 0;
    for (int cur = dst; cur != -1; cur = prev[cur])
        path[(*pathLen)++] = cur;

    // Reverse to get source-to-destination order
    for (int i = 0, j = *pathLen - 1; i < j; i++, j--) {
        int tmp = path[i];
        path[i] = path[j];
        path[j] = tmp;
    }
    return dist[dst];
}

// Function to calculate total cost of activities
int calculateTotalCost(struct TreeNode* node) {
    if (node == NULL) {
        return 0;
    }

    int totalCost = node->cost;

    struct TreeNode* current = node->activities;
    while (current != NULL) {
        totalCost += calculateTotalCost(current);
        current = current->next;
    }

    return totalCost;
}

// Function to calculate total days of activities
int calculateTotalDays(struct TreeNode* node) {
    if (node == NULL) {
        return 0;
    }

    int totalDays = node->days;

    struct TreeNode* current = node->activities;
    while (current != NULL) {
        totalDays += calculateTotalDays(current);
        current = current->next;
    }

    return totalDays;
}

int main() {
    printf("---------------TRAVEL PLAN OPTIMIZER - INDIA ONLY-----------------\n");

    struct TreeNode root;
    strncpy(root.name, "Trip", sizeof(root.name) - 1);
    root.name[sizeof(root.name) - 1] = '\0'; // Ensure null-termination
    root.days = 0;
    root.cost = 0;
    root.activities = NULL;
    root.next = NULL;
    int numDestinations = 0;
    char addAnotherDestination;

    do {
        printf("\n--- Enter Destination #%d ---\n", numDestinations + 1);
        char name[MAX_NAME_LENGTH];
        int days, cost;

        showSuggestions();

        int choice = -1;
        do {
            printf("Select a destination (0-%d): ", NUM_SUGGESTED);
            scanf("%d", &choice);
            if (choice < 0 || choice > NUM_SUGGESTED)
                printf("Error: Please enter a number between 0 and %d.\n", NUM_SUGGESTED);
        } while (choice < 0 || choice > NUM_SUGGESTED);
        getchar(); // consume newline

        if (choice == 0) {
            // Custom destination (India only)
            printf("Enter destination name (must be a place in India): ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = 0;

            do {
                printf("Enter number of days: ");
                scanf("%d", &days);
                if (days <= 0) printf("Error: Number of days must be greater than zero.\n");
            } while (days <= 0);

            do {
                printf("Enter cost: Rs.");
                scanf("%d", &cost);
                if (cost <= 0) printf("Error: Cost must be greater than zero.\n");
            } while (cost <= 0);

            getchar();
        } else {
            // Use suggested destination — allow user to override days/cost
            struct SuggestedDestination* s = &suggestedList[choice - 1];
            strncpy(name, s->name, MAX_NAME_LENGTH - 1);
            name[MAX_NAME_LENGTH - 1] = '\0';

            printf("Suggested: %s | Days: %d | Cost: Rs.%d\n", s->name, s->recommended_days, s->estimated_cost);
            printf("Use suggested values? (y/n): ");
            char useDefaults;
            scanf(" %c", &useDefaults);
            getchar();

            if (useDefaults == 'y' || useDefaults == 'Y') {
                days = s->recommended_days;
                cost = s->estimated_cost;
                printf("Using %d days and Rs.%d for %s.\n", days, cost, name);
            } else {
                do {
                    printf("Enter number of days: ");
                    scanf("%d", &days);
                    if (days <= 0) printf("Error: Number of days must be greater than zero.\n");
                } while (days <= 0);

                do {
                    printf("Enter cost: Rs.");
                    scanf("%d", &cost);
                    if (cost <= 0) printf("Error: Cost must be greater than zero.\n");
                } while (cost <= 0);

                getchar();
            }
        }

        struct TreeNode* destination = &root;
        int remainingDays = days;
        int remainingCost = cost;

        int numActivities = 0;
        char addAnotherActivity;

        do {
            char activityName[MAX_NAME_LENGTH];
            int activityDay, activityCost;

            printf("\n--- Enter Activity for %s ---\n", name);

            printf("Enter activity name: ");
            fgets(activityName, sizeof(activityName), stdin);
            activityName[strcspn(activityName, "\n")] = 0;

            do {
                printf("Enter day allocated (1-%d): ", remainingDays);
                scanf("%d", &activityDay);
                if (activityDay < 1 || activityDay > remainingDays) {
                    printf("Error: Invalid day. Please choose between 1 and %d.\n", remainingDays);
                }
            } while (activityDay < 1 || activityDay > remainingDays);

            do {
                printf("Enter cost: Rs.");
                scanf("%d", &activityCost);
                if (activityCost <= 0 || activityCost > remainingCost) {
                    printf("Error: Cost must be greater than zero and not exceed the remaining budget of Rs.%d.\n", remainingCost);
                }
            } while (activityCost <= 0 || activityCost > remainingCost);

            // Consume the newline character left in the input buffer
            getchar();

            addNode(destination, activityName, activityDay, activityCost, &remainingDays, &remainingCost);

            numActivities++;

            if (remainingDays == 0) {
                printf("No more days left for this destination.\n");
                break;
            } else if (remainingCost == 0) {
                printf("No more budget left for this destination.\n");
                break;
            } else if (remainingDays < 0 || remainingCost < 0) {
                printf("Error: Days or budget exceeded for this destination.\n");
                break;
            }

            printf("Remaining days: %d, Remaining cost: Rs.%d\n", remainingDays, remainingCost);

            printf("Add another activity for %s? (y/n): ", name);
            scanf(" %c", &addAnotherActivity);
            getchar(); // Consume the newline character

        } while ((addAnotherActivity == 'y' || addAnotherActivity == 'Y') && remainingDays > 0 && remainingCost > 0);

        numDestinations++;

        printf("Add another destination? (y/n): ");
        scanf(" %c", &addAnotherDestination);
        getchar(); // Consume the newline character

    } while (addAnotherDestination == 'y' || addAnotherDestination == 'Y');

    printf("\n--- Your Travel Itinerary ---\n");
    displayActivities(&root); // Display only activities

    if (numDestinations == 0) {
        printf("No destinations added.\n");
    }

    struct Destination destinations[MAX_DESTINATIONS];
    int numDestinationsInput = 0;

    // Adjacency matrix: INT_MAX = no direct connection, 0 on diagonal
    int distMatrix[MAX_DESTINATIONS][MAX_DESTINATIONS];
    for (int i = 0; i < MAX_DESTINATIONS; i++) {
        for (int j = 0; j < MAX_DESTINATIONS; j++) {
            distMatrix[i][j] = (i == j) ? 0 : INT_MAX;
        }
    }

    printf("\n--- Input Destinations for Route Optimization ---\n");
    printf("(Enter stops in order; distances link consecutive stops bidirectionally)\n");

    do {
        if (numDestinationsInput >= MAX_DESTINATIONS) {
            printf("Maximum destinations (%d) reached.\n", MAX_DESTINATIONS);
            break;
        }
        printf("\n--- Enter Stop #%d ---\n", numDestinationsInput + 1);

        showSuggestions();

        int choice = -1;
        do {
            printf("Select a destination (0-%d): ", NUM_SUGGESTED);
            scanf("%d", &choice);
            if (choice < 0 || choice > NUM_SUGGESTED)
                printf("Error: Please enter a number between 0 and %d.\n", NUM_SUGGESTED);
        } while (choice < 0 || choice > NUM_SUGGESTED);
        getchar();

        int segDist = 0;
        if (choice == 0) {
            printf("Enter destination name (must be a place in India): ");
            fgets(destinations[numDestinationsInput].name,
                  sizeof(destinations[numDestinationsInput].name), stdin);
            destinations[numDestinationsInput].name[
                strcspn(destinations[numDestinationsInput].name, "\n")] = 0;

            if (numDestinationsInput > 0) {
                do {
                    printf("Enter distance from %s to %s (km): ",
                           destinations[numDestinationsInput - 1].name,
                           destinations[numDestinationsInput].name);
                    scanf("%d", &segDist);
                    if (segDist < 0)
                        printf("Error: Distance must be non-negative.\n");
                } while (segDist < 0);
                getchar();
            }
        } else {
            struct SuggestedDestination* s = &suggestedList[choice - 1];
            strncpy(destinations[numDestinationsInput].name, s->name, MAX_NAME_LENGTH - 1);
            destinations[numDestinationsInput].name[MAX_NAME_LENGTH - 1] = '\0';

            if (numDestinationsInput > 0) {
                printf("Suggested distance from %s to %s: %d km. Use it? (y/n): ",
                       destinations[numDestinationsInput - 1].name,
                       s->name, s->distance_to_next);
                char useDefault;
                scanf(" %c", &useDefault);
                getchar();

                if (useDefault == 'y' || useDefault == 'Y') {
                    segDist = s->distance_to_next;
                } else {
                    do {
                        printf("Enter distance from %s to %s (km): ",
                               destinations[numDestinationsInput - 1].name, s->name);
                        scanf("%d", &segDist);
                        if (segDist < 0)
                            printf("Error: Distance must be non-negative.\n");
                    } while (segDist < 0);
                    getchar();
                }
            }
        }

        destinations[numDestinationsInput].distance = segDist;

        // Register bidirectional edge between this stop and the previous one
        if (numDestinationsInput > 0 && segDist > 0) {
            int prev = numDestinationsInput - 1;
            int cur = numDestinationsInput;
            distMatrix[prev][cur] = segDist;
            distMatrix[cur][prev] = segDist;
        }

        numDestinationsInput++;

        printf("Add another stop? (y/n): ");
        scanf(" %c", &addAnotherDestination);
        getchar();

    } while (addAnotherDestination == 'y' || addAnotherDestination == 'Y');

    // --- Source, optional halts, and destination ---
    char preferredSource[MAX_NAME_LENGTH];
    char preferredDestination[MAX_NAME_LENGTH];
    char halts[MAX_HALTS][MAX_NAME_LENGTH];
    int numHalts = 0;

    printf("\n--- Route: Source, Halts, and Destination ---\n");

    printf("Enter source: ");
    fgets(preferredSource, sizeof(preferredSource), stdin);
    preferredSource[strcspn(preferredSource, "\n")] = 0;

    char addHalt;
    printf("Add intermediate halts? (y/n): ");
    scanf(" %c", &addHalt);
    getchar();

    while ((addHalt == 'y' || addHalt == 'Y') && numHalts < MAX_HALTS) {
        printf("Enter halt #%d name: ", numHalts + 1);
        fgets(halts[numHalts], sizeof(halts[numHalts]), stdin);
        halts[numHalts][strcspn(halts[numHalts], "\n")] = 0;
        numHalts++;
        if (numHalts < MAX_HALTS) {
            printf("Add another halt? (y/n): ");
            scanf(" %c", &addHalt);
            getchar();
        }
    }

    printf("Enter destination: ");
    fgets(preferredDestination, sizeof(preferredDestination), stdin);
    preferredDestination[strcspn(preferredDestination, "\n")] = 0;

    // Build full route array: [source, halt1, ..., haltN, destination]
    int routeLen = numHalts + 2;
    char* route[MAX_HALTS + 2];
    route[0] = preferredSource;
    for (int i = 0; i < numHalts; i++)
        route[i + 1] = halts[i];
    route[routeLen - 1] = preferredDestination;

    printf("\n--- Optimized Route ---\n");
    int totalRouteDist = 0;
    int routeValid = 1;

    for (int leg = 0; leg < routeLen - 1; leg++) {
        int srcIdx = findDestByName(destinations, numDestinationsInput, route[leg]);
        int dstIdx = findDestByName(destinations, numDestinationsInput, route[leg + 1]);

        if (srcIdx == -1) {
            printf("Error: '%s' not found in stop list.\n", route[leg]);
            routeValid = 0;
            continue;
        }
        if (dstIdx == -1) {
            printf("Error: '%s' not found in stop list.\n", route[leg + 1]);
            routeValid = 0;
            continue;
        }

        int path[MAX_DESTINATIONS];
        int pathLen = 0;
        int legDist = runDijkstra(distMatrix, numDestinationsInput,
                                  srcIdx, dstIdx, path, &pathLen);

        if (legDist == INT_MAX) {
            printf("Leg %d: No path found from %s to %s.\n",
                   leg + 1, route[leg], route[leg + 1]);
            routeValid = 0;
            continue;
        }

        printf("Leg %d: ", leg + 1);
        for (int i = 0; i < pathLen; i++) {
            printf("%s", destinations[path[i]].name);
            if (i < pathLen - 1) printf(" -> ");
        }
        printf("  [%d km]\n", legDist);
        totalRouteDist += legDist;
    }

    if (routeValid)
        printf("Total route distance: %d km\n", totalRouteDist);

    // Calculate and display total cost and total days
    int totalCost = calculateTotalCost(&root);
    int totalDays = calculateTotalDays(&root);

    printf("\nTotal Cost of Activities: Rs.%d\n", totalCost);
    printf("Total Days of Activities: %d\n", totalDays);

    // Deallocate memory
    struct TreeNode* current = root.activities;
    while (current != NULL) {
        struct TreeNode* next = current->next;
        free(current);
        current = next;
    }

    return 0;
}
