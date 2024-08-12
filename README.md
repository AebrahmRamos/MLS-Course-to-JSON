# MLS Course to JSON

This script combines C and JavaScript to convert course information from MLS into JSON files, without relying on web automation libraries like Selenium or Puppeteer.
Tested and working on Mac, idk sa windows. Just have to modify some codes for it to work. 

# Features

- Fetches course data from MLS based on a list of courses in `subjects.txt`.
- Parses the downloaded HTML files and extracts course details.
- Generates a separate JSON file for each course under the `courseJSON` directory.
- Creates the `courseJSON` and `htmlFiles` directories automatically if they don't exist.

# Requirements

- **gcc**: A C compiler. You can usually install it through your system's package manager (search online for instructions).
- **Node.js and npm**: Download and install Node.js from the official website ([https://nodejs.org/](https://nodejs.org/)). npm is included with Node.js.

# Installation

1. Clone this repository.
2. Install the `jsdom` package:

   ```bash
   npm install jsdom
   ```
   
## Setting Cookies in fetchCourse.c
<img width="1413" alt="image" src="https://github.com/user-attachments/assets/3dfc0203-960d-4bcb-b882-088bdffcd788">

1. Open ([MLS View Course Offerings](https://enroll.dlsu.edu.ph/dlsu/view_course_offerings))
2. Open Inspect Element and navigate to Network tab
3. Enter your ID No. and Submit. You will now be redirected to the page asking for "Course"
4. Under Network tab find "view_course_offerings", go under Request Headers, and find cookie.
5. Do not copy the whole cookie, find the cf_clearance and copy starting from "cf_clearance = ..." until the end of the clearance string
   <img width="655" alt="image" src="https://github.com/user-attachments/assets/6a5bfa34-d3a1-496f-8e2c-4bf3eb2b13f8">

7. Replace the value of cookie variable in `fetchCourse.c` with the copied cookie
8. You're ready to go



# Usage

1. **Fetch course data:**
   ```bash
   gcc fetchCourse.c -o fetchCourse && ./fetchCourse
   ```
   This compiles `fetchCourse.c` and executes the resulting `fetchCourse` program. It retrieves data for courses listed in `subjects.txt` and generates HTML files in the `htmlFiles` directory.

2. **Extract course information:**
   ```bash
   node index.js
   ```
   This runs the `index.js` script, which parses the HTML files and creates JSON files for each course in the `courseJSON` directory.

# Course Text Files

Currently, the script supports parsing courses categorized under "CT-Department-Courses", "ST-Department-Courses", and "IT-Department-Courses" in `subjects.txt`. You can modify `fetchCourse.c` to target different list formats or sources for course data.

# Notes

- The script creates the `courseJSON` and `htmlFiles` directories if they don't exist.
- Ensure `subjects.txt` is present in the project directory with valid course identifiers.


