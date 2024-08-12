# MLS-Course-to-JSON
Simple script C and Javascript that obtains a list of courses from MLS and parses into a JSON file without using Selenium, Puppeteer, or any web automation library/framework. Instead, it uses curl request to MLS' server.

# Requirements
- gcc (search google)
- Install JSDom by running
`npm install jsdom`

# Guide
1. Run fetchCourse.c `gcc fetchCourse.c -o fetchCourse && ./fetchCourse`
  `fetchCourse.c` will fetch course data from MLS based from the courses listed in `subjects.txt`. html files will be generated in `htmlFiles` folder
2. Run index.js `node index.js`
3. JSON files of each course subjects will then be saved under courseJSON folder
