const fs = require('fs');
const { JSDOM } = require('jsdom');
const outputDir = 'courseJSON';
const path = require('path');

if (!fs.existsSync(outputDir)) {
    fs.mkdirSync(outputDir);
}

const lines = getFileNamesWithoutExtension('./htmlFiles/');


lines.forEach((line) => {
    if (line.trim()) {
        const htmlFilePath = path.join('./htmlFiles', `${line.trim()}.html`);
        
        if (fs.existsSync(htmlFilePath)) {
          try {
            const html = fs.readFileSync(htmlFilePath, 'utf8');
            const subjects = extractSubjectInformation(html);
            const outputFilePath = path.join(outputDir, `${line.trim()}.json`);
            fs.writeFileSync(outputFilePath, JSON.stringify(subjects, null, 2), 'utf8');
            fs.unlinkSync(htmlFilePath);
            
          } catch (err) {
              console.error(`Error reading or deleting file ${htmlFilePath}:`, err);
          }
      } else {
          console.error(`File not found: ${htmlFilePath}`);
      }
    }
});

console.log('Subject information has been extracted and saved to JSON files.');

function extractSubjectInformation(html) {
    const { window } = new JSDOM(html);
    const $ = (selector) => [...window.document.querySelectorAll(selector)];
  
    const subjects = [];
    let currentSubject = null;
  
    const rows = $('tr');
    for (const row of rows) {
      const cells = [...row.querySelectorAll('td')];
  
      if (cells.length === 9 && cells[0].textContent.trim() !== 'Class Nbr' && cells[0].textContent.trim() !== '' && cells[1].textContent.trim() !== '' && cells[2].textContent.trim() !== '') {
        currentSubject = {
          classNbr: cells[0].textContent.trim(),
          course: cells[1].textContent.trim(),
          section: cells[2].textContent.trim(),
          schedules: [
            {
              day: cells[3].textContent.trim(),
              time: cells[4].textContent.trim(),
              room: cells[5].textContent.trim() ? cells[5].textContent.trim() : null,
            }
          ],
          enrollmentCap: cells[6].textContent.trim(),
          enrolled: cells[7].textContent.trim(),
          remarks: cells[8].textContent.trim(),
          professor: null,
        };
        subjects.push(currentSubject);
      } else if (cells.length === 1 && cells[0].getAttribute('colspan') === '6') {
        if (currentSubject) {
          currentSubject.professor = cells[0].textContent.trim();
        }
      } else if (cells.length >= 4 && currentSubject) {
        const day = cells[3].textContent.trim();
        const isValidDay = ['M', 'T', 'W', 'H', 'F', 'S'].includes(day);
  
        if (isValidDay) {
          const schedule = {
            day: day,
            time: cells[4].textContent.trim(),
            room: cells[5].textContent.trim(),
          };
          currentSubject.schedules.push(schedule);
        }
      }
    }
    return subjects.filter((subject) => subject.classNbr !== 'Class Nbr');
  }

  function getFileNamesWithoutExtension(directoryPath) {
    const files = fs.readdirSync(directoryPath);
    return files.map(file => path.parse(file).name);
  }
