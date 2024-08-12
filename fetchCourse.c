#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_LINE_LENGTH 1024

int extract_form_from_html(const char* input_filename, const char* output_filename) {
    FILE *input_file, *output_file;
    char line[MAX_LINE_LENGTH];
    int inside_form = 0;
    int form_closed = 0;

    input_file = fopen(input_filename, "r");
    if (input_file == NULL) {
        printf("Error opening input file %s\n", input_filename);
        return 1;
    }

    output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        printf("Error opening output file %s\n", output_filename);
        fclose(input_file);
        return 1;
    }

    fputs("<html><head></head><body>", output_file);
    while (fgets(line, MAX_LINE_LENGTH, input_file) != NULL) {
        if (strstr(line, "<FORM ACTION=\"view_course_offerings\" METHOD=\"POST\">") != NULL) {
            inside_form = 1;
            form_closed = 0;
            fputs(line, output_file);
        } else if (strstr(line, "</FORM>") != NULL) {
            fputs(line, output_file);
            form_closed++;
            if (form_closed == 2) {
                inside_form = 0;
            }
        } else if (inside_form) {
            fputs(line, output_file);
        }
    }
    fputs("</body></html>", output_file);
    fclose(input_file);
    fclose(output_file);

    return 0;
}

int fetch(char* subject, char* cookie, char* outputFile) {
    const char* command_template = "curl 'https://enroll.dlsu.edu.ph/dlsu/view_course_offerings' \
        -H 'accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7' \
        -H 'accept-language: en-US,en;q=0.9' \
        -H 'cache-control: max-age=0' \
        -H 'content-type: application/x-www-form-urlencoded' \
        -H 'cookie: %s' \
        -H 'dnt: 1' \
        -H 'origin: https://enroll.dlsu.edu.ph' \
        -H 'priority: u=0, i' \
        -H 'referer: https://enroll.dlsu.edu.ph/dlsu/view_course_offerings' \
        -H 'sec-ch-ua: \"Chromium\";v=\"127\", \"Not)A;Brand\";v=\"99\"' \
        -H 'sec-ch-ua-arch: \"arm\"' \
        -H 'sec-ch-ua-bitness: \"64\"' \
        -H 'sec-ch-ua-full-version: \"127.0.6533.73\"' \
        -H 'sec-ch-ua-full-version-list: \"Chromium\";v=\"127.0.6533.73\", \"Not)A;Brand\";v=\"99.0.0.0\"' \
        -H 'sec-ch-ua-mobile: ?0' \
        -H 'sec-ch-ua-model: \"\"' \
        -H 'sec-ch-ua-platform: \"macOS\"' \
        -H 'sec-ch-ua-platform-version: \"14.1.2\"' \
        -H 'sec-fetch-dest: document' \
        -H 'sec-fetch-mode: navigate' \
        -H 'sec-fetch-site: same-origin' \
        -H 'sec-fetch-user: ?1' \
        -H 'upgrade-insecure-requests: 1' \
        -H 'user-agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/127.0.0.0 Safari/537.36' \
        --data-raw 'p_course_code=%s&p_option=all&p_button=Search&p_id_no=12216496&p_button=Search' -o %s";
    
    int command_length = strlen(command_template) + strlen(subject) + strlen(cookie) + strlen(outputFile) + 1;
    char* command = malloc(command_length);
    sprintf(command, command_template, cookie, subject, outputFile);

    int result = system(command);
    if (result == -1) {
        printf("Error executing command\n");
        free(command);
    }

    free(command);

    char tableFile[40];
    sprintf(tableFile, "htmlFiles/%s.html", subject);
    if (extract_form_from_html(outputFile, tableFile) != 0) {
        printf("Error extracting form from HTML\n");
        return 0;
    }
    FILE *table_file = fopen(tableFile, "r");
    if (table_file == NULL) {
        printf("Error opening table file\n");
        return 0;
    }

    int line_count = 0;
    char ch;
    while ((ch = fgetc(table_file)) != EOF) {
        if (ch == '\n') {
            line_count++;
        }
    }

    fclose(table_file);
    printf("Number of lines in table file: %d\n", line_count);
    if (line_count <= 85)
    {
        remove(tableFile);
        return 0;
    }
    
    remove(outputFile);
    return 1;
}

int main() {
    char cookie[300] = "your_cookie_here";
    char outputFile[40] = "htmlFiles/offering.html";

    struct stat st = {0};
    if (stat("htmlFiles", &st) == -1) {
        mkdir("htmlFiles", 0700);
    }

    FILE *subjects_file = fopen("subjects.txt", "r");
    if (subjects_file == NULL) {
        printf("Error opening subjects file\n");
        return 1;
    }

    char subject[11];

    while (fgets(subject, sizeof(subject), subjects_file) != NULL) {
        subject[strcspn(subject, "\n")] = '\0';

        if (fetch(subject, cookie, outputFile) == 1) {
            printf("Subject %s offered\n", subject);

        }
    }

    fclose(subjects_file);

    return 0;
}