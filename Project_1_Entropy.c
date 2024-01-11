#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define ALPHABET_SIZE 27

int countOccurrences(FILE *file, int *count) {
    int c;
    while ((c = fgetc(file)) != EOF) {
        // Μετατροπή όλων των χαρακτήρων σε πεζά
        c = tolower(c);
        if ((c >= 'a' && c <= 'z') || c == ' ') {
            if (c == ' ') {
                count[ALPHABET_SIZE - 1]++; // Αύξηση εμφανίσεων του κενού
                // Skip consecutive spaces
                while ((c = fgetc(file)) == ' ');
                if (c != EOF) {
                    fseek(file, -1, SEEK_CUR); // Επανατοποθετεί τον δείκτη του αρχείου μία θέση πίσω όταν βρει χαρακτήρα δίαφορο του κενού
                }
                printf(" "); // Τυπώνει το περιεχόμενο του αρχείου
            } else {
                printf("%c", c); // Τυπώνει το περιεχόμενο του αρχείου
                count[c - 'a']++;
            }
        }
    }
    int total = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        total += count[i]; // Εύρεση του συνόλου των χαρακτήρων
    }
    return total;
}

double calculateEntropy(int *count, int total) { // Υπολογισμός εντροπίας
    double entropy = 0.0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (count[i] > 0) {
            double probability = (double)count[i] / total;
            entropy -= probability * log2(probability);
        }
    }
    return entropy;
}

int main(int argc, char *argv[]) {
    if (argc != 2) { // Αν δεν δωθεί το όνομα του εκτελέσιμου και το όνομα του αρχείου κειμένου
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r"); // Άνοιγμα αρχείου για διάβασμα
    if (file == NULL) { // Αν δεν είναι δυνατό το άνοιγμα του αρχείου
        printf("Error opening the file.\n");
        return 1;
    }

    int count[ALPHABET_SIZE] = {0};
    int total = countOccurrences(file, count); // Υπολογισμός της εμφάνισης κάθε χαρακτήρα

    if (total == 0) { // Το αρχείο είναι κενό ή σε άλλη γλώσσα (μη λατινικοί χαρακτήρες και μη ύπαρξη κενού)
        printf("The file is empty or doesn't contain relevant characters.\n");
        fclose(file);
        return 1;
    }

    double entropy = calculateEntropy(count, total); // Υπολογισμός εντροπίας
    printf("\nEntropy: %f\n", entropy);

    fclose(file); // Κλείσιμο αρχείου
    return 0;
}
