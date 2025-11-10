#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_ROOMS 100

struct Room {
    int roomNumber;
    char roomType[20];
    float pricePerNight;
    int isBooked;
    char customerName[50];
    int daysBooked;
    char aadhaar[13]; // 12 digits + null
};

struct Room hotel[MAX_ROOMS];
int roomCount = 0;

/* ---------- Helpers ---------- */

const char* typeForRoom(int num) {
    if (num >= 100 && num <= 129) return "Single";
    if (num >= 130 && num <= 169) return "Double";
    if (num >= 170 && num <= 199) return "Suite";
    return "Unknown";
}

float priceForType(const char* t) {
    if (strcmp(t, "Single") == 0) return 1999.0f;
    if (strcmp(t, "Double") == 0) return 2999.0f;
    if (strcmp(t, "Suite")  == 0) return 5999.0f;
    return 0.0f;
}

int validateAadhaar(const char *s) {
    if (!s) return 0;
    int len = (int)strlen(s);
    if (len != 12) return 0;
    for (int i = 0; i < 12; i++) {
        if (!isdigit((unsigned char)s[i])) return 0;
    }
    return 1;
}

int availableCount(void) {
    int c = 0;
    for (int i = 0; i < roomCount; i++) if (!hotel[i].isBooked) c++;
    return c;
}

int findRoomIndex(int num) {
    for (int i = 0; i < roomCount; i++) if (hotel[i].roomNumber == num) return i;
    return -1;
}

/* ---------- Availability Ranges ---------- */

int availableCountInRange(int start, int end) {
    int c = 0;
    for (int num = start; num <= end; num++) {
        int idx = findRoomIndex(num);
        if (idx != -1 && !hotel[idx].isBooked) c++;
    }
    return c;
}

void printAvailableRangesForType(int start, int end, const char *label) {
    int count = availableCountInRange(start, end);
    printf("%s (%d available): ", label, count);

    int inRun = 0;
    int runStart = -1;

    for (int num = start; num <= end; num++) {
        int idx = findRoomIndex(num);
        int free = (idx != -1 && !hotel[idx].isBooked);

        if (free && !inRun) {
            inRun = 1;
            runStart = num;
        } else if (!free && inRun) {
            inRun = 0;
            if (runStart == num - 1) {
                printf("%d, ", runStart);
            } else {
                printf("%d-%d, ", runStart, num - 1);
            }
        }
    }
    // close trailing run, if any
    if (inRun) {
        if (runStart == end) {
            printf("%d, ", runStart);
        } else {
            printf("%d-%d, ", runStart, end);
        }
    }

    if (count == 0) {
        printf("None");
    } else {
        // trim trailing comma+space visually by adding a newline after
        // (no need to backspace in console)
    }
    printf("\n");
}

void showAvailabilitySummary(void) {
    printf("\n--- Availability Summary ---\n");
    printAvailableRangesForType(100, 129, "Single");
    printAvailableRangesForType(130, 169, "Double");
    printAvailableRangesForType(170, 199, "Suite");
}

/* ---------- Core features ---------- */

void initRooms(void) {
    // Pre-allocate: 100–129 Single, 130–169 Double, 170–199 Suite
    for (int num = 100; num <= 199; num++) {
        const char* t = typeForRoom(num);
        if (strcmp(t, "Unknown") == 0) continue;

        struct Room r;
        r.roomNumber = num;
        strncpy(r.roomType, t, sizeof(r.roomType));
        r.roomType[sizeof(r.roomType)-1] = '\0';
        r.pricePerNight = priceForType(t);
        r.isBooked = 0;
        r.customerName[0] = '\0';
        r.daysBooked = 0;
        r.aadhaar[0] = '\0';

        hotel[roomCount++] = r;
        if (roomCount >= MAX_ROOMS) break;
    }
}

void displayRooms(void) {
    if (roomCount == 0) {
        printf("No rooms configured.\n");
        return;
    }

    printf("\n--- Room List (%d total, %d available) ---\n", roomCount, availableCount());
    printf("Room  | Type    | Price/Night | Status     | Guest (if booked)\n");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < roomCount; i++) {
        printf("%-5d | %-7s | ₹%-10.2f | %-10s",
               hotel[i].roomNumber,
               hotel[i].roomType,
               hotel[i].pricePerNight,
               hotel[i].isBooked ? "Booked" : "Available");
        if (hotel[i].isBooked) {
            printf(" | %s", hotel[i].customerName);
        }
        printf("\n");
    }
}

void bookRoom(void) {
    int num, days;
    char name[50];
    char aadhaar[13];

    printf("Enter Room Number to Book: ");
    if (scanf("%d", &num) != 1) { printf("Invalid input.\n"); return; }

    int idx = findRoomIndex(num);
    if (idx == -1) { printf("Ahh! Room not found.\n"); return; }

    if (hotel[idx].isBooked) {
        printf("Sorry, this room is already booked.\n");
        return;
    }

    printf("Enter Customer Name: ");
    getchar(); // consume leftover newline
    if (fgets(name, sizeof(name), stdin) == NULL) { printf("Invalid input.\n"); return; }
    size_t ln = strlen(name);
    if (ln > 0 && name[ln-1] == '\n') name[ln-1] = '\0';

    printf("Enter Number of Days: ");
    if (scanf("%d", &days) != 1 || days <= 0) {
        printf("Invalid number of days.\n");
        return;
    }

    printf("Enter 12-digit Aadhaar Number: ");
    if (scanf("%12s", aadhaar) != 1 || !validateAadhaar(aadhaar)) {
        printf("Invalid Aadhaar. It must be exactly 12 digits.\n");
        return;
    }

    hotel[idx].isBooked = 1;
    strncpy(hotel[idx].customerName, name, sizeof(hotel[idx].customerName));
    hotel[idx].customerName[sizeof(hotel[idx].customerName)-1] = '\0';
    hotel[idx].daysBooked = days;
    strncpy(hotel[idx].aadhaar, aadhaar, sizeof(hotel[idx].aadhaar));
    hotel[idx].aadhaar[sizeof(hotel[idx].aadhaar)-1] = '\0';

    printf("Room %d (%s) booked successfully for %s for %d days!\n",
           num, hotel[idx].roomType, name, days);
}

void checkoutRoom(void) {
    int num;
    printf("Enter Room Number for Checkout: ");
    if (scanf("%d", &num) != 1) { printf("Invalid input.\n"); return; }

    int idx = findRoomIndex(num);
    if (idx == -1) { printf("Aww! Room not found.\n"); return; }

    if (!hotel[idx].isBooked) {
        printf("Yayy! Room %d is not booked.\n", num);
        return;
    }

    float total = hotel[idx].daysBooked * hotel[idx].pricePerNight;
    printf("\nCustomer %s checked out.\n", hotel[idx].customerName);
    printf("Total Bill: ₹%.2f\n", total);

    // Clear all booking info including Aadhaar
    hotel[idx].isBooked = 0;
    hotel[idx].customerName[0] = '\0';
    hotel[idx].daysBooked = 0;
    hotel[idx].aadhaar[0] = '\0';

    printf("Aadhaar data deleted securely for room %d.\n", num);
}

void searchRoom(void) {
    int num;
    printf("Enter Room Number to Search: ");
    if (scanf("%d", &num) != 1) { printf("Invalid input.\n"); return; }

    int idx = findRoomIndex(num);
    if (idx == -1) { printf("Aww! Room not found.\n"); return; }

    printf("\nRoom Number: %d\n", hotel[idx].roomNumber);
    printf("Type: %s\n", hotel[idx].roomType);
    printf("Price per Night: ₹%.2f\n", hotel[idx].pricePerNight);
    printf("Status: %s\n", hotel[idx].isBooked ? "Booked" : "Available");
    if (hotel[idx].isBooked) {
        printf("Booked By: %s\n", hotel[idx].customerName);
        printf("Days Booked: %d\n", hotel[idx].daysBooked);
        // Aadhaar intentionally not displayed for privacy
    }
}

/* ---------- Main ---------- */

int main(void) {
    int choice;

    initRooms();
    
        printf("\n===== HOTEL ROOM MANAGEMENT SYSTEM =====\n");
        printf("Total Rooms: %d | Available: %d\n", roomCount, availableCount());
    while (1) {
        showAvailabilitySummary();
        printf("\n----------------------------------------------------------------------------------\n");
        printf("1. Display All Rooms | 2. Book Room | 3. Checkout Room | 4. Search Room | 5. Exit");
        printf("\n>> Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf(">> Invalid input. Please try again.\n");
            int ch; while ((ch = getchar()) != '\n' && ch != EOF);
            continue;
        }

        switch (choice) {
            case 1:
                displayRooms();
                break;
            case 2:
                bookRoom();
                break;
            case 3:
                checkoutRoom();
                break;
            case 4:
                searchRoom();
                break;
            case 5:
                printf("\033[1;32m");
                printf("\n===============================================\n");
                printf("THANK YOU FOR CHOOSING US! ✨\n");
                printf("Have a wonderful day ahead. Stay happy!  😊\n");
                printf("-----------------------------------------------\n");
                printf(" With regards,\n");
                printf("      Team Salim & Rohit \n");
                printf("===============================================\n\n");
                printf("\033[0m");
                return 0;
            default:
                printf(">> Invalid choice. Please try again.\n");
        }
    }
}