# Hotel Room Booking Management System  

![Made with C](https://img.shields.io/badge/Made%20with-C-blue?style=for-the-badge&logo=c)
![Status](https://img.shields.io/badge/Status-Completed-success?style=for-the-badge)

> **A simple and efficient C program for managing hotel room bookings, checkouts, and availability.**

---

## Features
✅ Book and checkout rooms  
✅ View all available rooms  
✅ Search rooms by number  
✅ Auto-calculate total bill  
✅ Aadhaar validation (12-digit)  
✅ Privacy-first: Guest info cleared after checkout  

---

## Room Categories
| Type   | Room Range | Price/Night |
|--------|-------------|-------------|
| 🛏️ Single | 100–129 | ₹1999 |
| 🛋️ Double | 130–169 | ₹2999 |
| 🏠 Suite  | 170–199 | ₹5999 |

---

## Tech Info
- **Language:** C  
- **Total Rooms:** 100  
- **Data Structure:** Array of `struct Room`  

```c
struct Room {
  int roomNumber;
  char roomType[20];
  float pricePerNight;
  int isBooked;
  char customerName[50];
  int daysBooked;
  char aadhaar[13];
};
```
---

## Run Locally
🧩 Clone the Repository
```c
git clone https://github.com/saleeem07/HotelRoom_Booking_Management_System.git
cd HotelRoom_Booking_Management_System
```
🛠️ Compile & Run
```c
gcc hotel_management.c -o hotel
./hotel
```

---

## Menu Options
```c
1. Display All Rooms  
2. Book Room  
3. Checkout Room  
4. Search Room  
5. Exit
```

---

## Developers
1. Rohit – Lead Developer
2. Salim – Co-Developer

“Thank you for choosing us! Have a wonderful stay 😊”
   
---

## Future Plans
🔹 File-based data storage

🔹 Admin login system

🔹 GUI or Web interface

⭐ If you like this project, don’t forget to give it a star! ⭐ Because true Devs don’t just write code — they build experiences.








