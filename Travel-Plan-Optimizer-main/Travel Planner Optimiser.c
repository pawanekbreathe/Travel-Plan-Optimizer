#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

#define MAX_NAME_LENGTH 50
#define MAX_DESTINATIONS 10
#define NUM_SUGGESTED 8

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

// Dijkstra's algorithm to find the shortest path and distance
void dijkstra(struct Destination destinations[MAX_DESTINATIONS], int numDestinations, char* source, char* destination) {
    int dist[MAX_DESTINATIONS];
    int visited[MAX_DESTINATIONS];
    int prev[MAX_DESTINATIONS];

    // Initialize distances and visited flags
    for (int i = 0; i < numDestinations; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
        prev[i] = -1;
    }

    // Find the source and destination indices
    int sourceIndex = -1;
    int destIndex = -1;
    for (int i = 0; i < numDestinations; i++) {
        if (strcmp(destinations[i].name, source) == 0) {
            sourceIndex = i;
        }
        if (strcmp(destinations[i].name, destination) == 0) {
            destIndex = i;
        }
    }

    if (sourceIndex == -1 || destIndex == -1) {
        printf("Source or destination not found in the list of destinations.\n");
        return;
    }

    dist[sourceIndex] = 0;

    for (int count = 0; count < numDestinations - 1; count++) {
        int u = -1;
        int minDist = INT_MAX;

        for (int v = 0; v < numDestinations; v++) {
            if (!visited[v] && dist[v] < minDist) {
                u = v;
                minDist = dist[v];
            }
        }

        visited[u] = 1;

        for (int v = 0; v < numDestinations; v++) {
            if (!visited[v] && destinations[u].distance + dist[u] < dist[v]) {
                dist[v] = destinations[u].distance + dist[u];
                prev[v] = u;
            }
        }
    }

    // Print the optimized distance
    printf("Optimized Distance for the user's route from %s to %s: %d\n", source, destination, dist[destIndex]);

    // Reconstruct and print the path using prev[]
    int path[MAX_DESTINATIONS];
    int pathLen = 0;
    for (int cur = destIndex; cur != -1; cur = prev[cur]) {
        path[pathLen++] = cur;
    }
    printf("Path: ");
    for (int i = pathLen - 1; i >= 0; i--) {
        printf("%s", destinations[path[i]].name);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
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

    printf("\n--- Input Destination Information for Route Optimization ---\n");
    printf("(Distances between consecutive destinations will be used for Dijkstra's algorithm)\n");

    do {
        printf("\n--- Enter Destination #%d ---\n", numDestinationsInput + 1);

        showSuggestions();

        int choice = -1;
        do {
            printf("Select a destination (0-%d): ", NUM_SUGGESTED);
            scanf("%d", &choice);
            if (choice < 0 || choice > NUM_SUGGESTED)
                printf("Error: Please enter a number between 0 and %d.\n", NUM_SUGGESTED);
        } while (choice < 0 || choice > NUM_SUGGESTED);
        getchar();

        if (choice == 0) {
            printf("Enter destination name (must be a place in India): ");
            fgets(destinations[numDestinationsInput].name, sizeof(destinations[numDestinationsInput].name), stdin);
            destinations[numDestinationsInput].name[strcspn(destinations[numDestinationsInput].name, "\n")] = 0;

            do {
                printf("Enter distance to the next destination (km): ");
                scanf("%d", &destinations[numDestinationsInput].distance);
                if (destinations[numDestinationsInput].distance < 0)
                    printf("Error: Distance must be non-negative.\n");
            } while (destinations[numDestinationsInput].distance < 0);

            getchar();
        } else {
            struct SuggestedDestination* s = &suggestedList[choice - 1];
            strncpy(destinations[numDestinationsInput].name, s->name, MAX_NAME_LENGTH - 1);
            destinations[numDestinationsInput].name[MAX_NAME_LENGTH - 1] = '\0';

            printf("Suggested distance to next destination: %d km. Use it? (y/n): ", s->distance_to_next);
            char useDefault;
            scanf(" %c", &useDefault);
            getchar();

            if (useDefault == 'y' || useDefault == 'Y') {
                destinations[numDestinationsInput].distance = s->distance_to_next;
            } else {
                do {
                    printf("Enter distance to the next destination (km): ");
                    scanf("%d", &destinations[numDestinationsInput].distance);
                    if (destinations[numDestinationsInput].distance < 0)
                        printf("Error: Distance must be non-negative.\n");
                } while (destinations[numDestinationsInput].distance < 0);
                getchar();
            }
        }

        numDestinationsInput++;

        printf("Add another destination? (y/n): ");
        scanf(" %c", &addAnotherDestination);
        getchar();

    } while (addAnotherDestination == 'y' || addAnotherDestination == 'Y');

    char preferredSource[MAX_NAME_LENGTH];
    char preferredDestination[MAX_NAME_LENGTH];

    // Input user's preferred source and destination
    printf("\n--- User's Preferred Source and Destination ---\n");

    printf("Enter preferred source: ");
    fgets(preferredSource, sizeof(preferredSource), stdin);
    preferredSource[strcspn(preferredSource, "\n")] = 0;

    printf("Enter preferred destination: ");
    fgets(preferredDestination, sizeof(preferredDestination), stdin);
    preferredDestination[strcspn(preferredDestination, "\n")] = 0;

    // Add the optimized distance calculation here
    dijkstra(destinations, numDestinationsInput, preferredSource, preferredDestination);

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
