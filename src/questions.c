#include "question.h"

static const Question QUESTIONS[] = {
    {
        "Which header file is commonly used for input and output functions like printf and scanf?",
        {"<math.h>", "<stdio.h>", "<string.h>", "<stdlib.h>"},
        1,
        CATEGORY_C_PROGRAMMING,
        "printf and scanf are declared in <stdio.h>."
    },
    {
        "What is the correct way to declare a constant integer in C?",
        {"int constValue = 10;", "constant int value = 10;", "const int value = 10;", "immutable int value = 10;"},
        2,
        CATEGORY_C_PROGRAMMING,
        "The const keyword makes the variable read-only after initialization."
    },
    {
        "Which operator is used to access the value stored at a pointer address?",
        {"&", "*", "%", "#"},
        1,
        CATEGORY_C_PROGRAMMING,
        "The dereference operator * reads or writes the value pointed to by a pointer."
    },
    {
        "What does strlen return?",
        {"The size of the array in bytes", "The number of characters before the null terminator", "The memory address of the string", "The number of words in the string"},
        1,
        CATEGORY_C_PROGRAMMING,
        "strlen counts characters until it reaches the null terminator."
    },
    {
        "Which loop is guaranteed to execute its body at least once?",
        {"for", "while", "do-while", "foreach"},
        2,
        CATEGORY_C_PROGRAMMING,
        "A do-while loop checks the condition after running the loop body once."
    },
    {
        "Which keyword is used to define a custom data type made of multiple members?",
        {"union", "typedef", "struct", "enum"},
        2,
        CATEGORY_C_PROGRAMMING,
        "A struct groups related members together into one custom type."
    },
    {
        "What is the array index of the first element in C?",
        {"0", "1", "-1", "Depends on the compiler"},
        0,
        CATEGORY_C_PROGRAMMING,
        "C arrays are zero-indexed."
    },
    {
        "Which function is typically used to dynamically allocate memory for an array?",
        {"calloc or malloc", "printf", "free", "realloc only"},
        0,
        CATEGORY_C_PROGRAMMING,
        "malloc and calloc allocate memory dynamically on the heap."
    },
    {
        "What is the purpose of the break statement in a switch block?",
        {"It skips the next case label", "It terminates the program", "It exits the switch block", "It restarts the switch block"},
        2,
        CATEGORY_C_PROGRAMMING,
        "Without break, execution can fall through into the next case."
    },
    {
        "Which function is used to compare two strings in C?",
        {"strcpy", "strcmp", "strcat", "strrev"},
        1,
        CATEGORY_C_PROGRAMMING,
        "strcmp compares two strings lexicographically."
    },
    {
        "What does EOF usually indicate while reading input?",
        {"A buffer overflow", "End of file or input stream", "An empty string", "A syntax error"},
        1,
        CATEGORY_C_PROGRAMMING,
        "EOF signals there is no more input available from the stream."
    },
    {
        "Which storage class specifier keeps a local variable's value between function calls?",
        {"extern", "register", "static", "auto"},
        2,
        CATEGORY_C_PROGRAMMING,
        "A static local variable is initialized once and retains its value across calls."
    },
    {
        "Which is the largest ocean on Earth?",
        {"Atlantic Ocean", "Indian Ocean", "Arctic Ocean", "Pacific Ocean"},
        3,
        CATEGORY_GENERAL_KNOWLEDGE,
        "The Pacific Ocean is the largest and deepest ocean on Earth."
    },
    {
        "Who wrote the play Romeo and Juliet?",
        {"William Shakespeare", "Charles Dickens", "Jane Austen", "Mark Twain"},
        0,
        CATEGORY_GENERAL_KNOWLEDGE,
        "Romeo and Juliet is one of Shakespeare's most famous tragedies."
    },
    {
        "Which country is known as the Land of the Rising Sun?",
        {"China", "Japan", "Thailand", "South Korea"},
        1,
        CATEGORY_GENERAL_KNOWLEDGE,
        "Japan is widely called the Land of the Rising Sun."
    },
    {
        "How many continents are there on Earth?",
        {"5", "6", "7", "8"},
        2,
        CATEGORY_GENERAL_KNOWLEDGE,
        "The common model counts 7 continents."
    },
    {
        "Which gas do plants primarily absorb from the atmosphere?",
        {"Oxygen", "Nitrogen", "Carbon dioxide", "Helium"},
        2,
        CATEGORY_GENERAL_KNOWLEDGE,
        "Plants use carbon dioxide during photosynthesis."
    },
    {
        "Which is the smallest prime number?",
        {"0", "1", "2", "3"},
        2,
        CATEGORY_GENERAL_KNOWLEDGE,
        "2 is the smallest and the only even prime number."
    },
    {
        "Which instrument has 88 keys on a standard model?",
        {"Violin", "Piano", "Flute", "Sitar"},
        1,
        CATEGORY_GENERAL_KNOWLEDGE,
        "A standard piano has 88 keys."
    },
    {
        "What is the capital city of Australia?",
        {"Sydney", "Canberra", "Melbourne", "Perth"},
        1,
        CATEGORY_GENERAL_KNOWLEDGE,
        "Canberra is the capital of Australia."
    },
    {
        "Which festival is known as the Festival of Lights in India?",
        {"Holi", "Pongal", "Diwali", "Onam"},
        2,
        CATEGORY_GENERAL_KNOWLEDGE,
        "Diwali is widely celebrated as the Festival of Lights."
    },
    {
        "Who painted the Mona Lisa?",
        {"Vincent van Gogh", "Leonardo da Vinci", "Pablo Picasso", "Claude Monet"},
        1,
        CATEGORY_GENERAL_KNOWLEDGE,
        "The Mona Lisa was painted by Leonardo da Vinci."
    },
    {
        "Which planet is famous for its rings?",
        {"Mars", "Venus", "Saturn", "Mercury"},
        2,
        CATEGORY_GENERAL_KNOWLEDGE,
        "Saturn is famous for its prominent ring system."
    },
    {
        "What is the currency of Japan?",
        {"Won", "Yuan", "Yen", "Ringgit"},
        2,
        CATEGORY_GENERAL_KNOWLEDGE,
        "Japan uses the yen as its currency."
    },
    {
        "What is the chemical symbol for water?",
        {"O2", "CO2", "NaCl", "H2O"},
        3,
        CATEGORY_SCIENCE,
        "Water is made of two hydrogen atoms and one oxygen atom: H2O."
    },
    {
        "What force pulls objects toward the Earth?",
        {"Magnetism", "Friction", "Gravity", "Radiation"},
        2,
        CATEGORY_SCIENCE,
        "Gravity attracts objects with mass toward each other."
    },
    {
        "Which part of the cell contains genetic material?",
        {"Cell wall", "Nucleus", "Ribosome", "Cytoplasm"},
        1,
        CATEGORY_SCIENCE,
        "The nucleus stores DNA in most eukaryotic cells."
    },
    {
        "What is the speed of light closest to in vacuum?",
        {"300,000 km/s", "30,000 km/s", "3,000 km/s", "3 km/s"},
        0,
        CATEGORY_SCIENCE,
        "The speed of light in vacuum is about 300,000 kilometers per second."
    },
    {
        "Which gas is most abundant in Earth's atmosphere?",
        {"Oxygen", "Nitrogen", "Carbon dioxide", "Hydrogen"},
        1,
        CATEGORY_SCIENCE,
        "Nitrogen makes up about 78 percent of Earth's atmosphere."
    },
    {
        "Which organ pumps blood through the human body?",
        {"Brain", "Liver", "Heart", "Lung"},
        2,
        CATEGORY_SCIENCE,
        "The heart circulates blood throughout the body."
    },
    {
        "What is the boiling point of water at sea level in Celsius?",
        {"50", "75", "90", "100"},
        3,
        CATEGORY_SCIENCE,
        "At standard atmospheric pressure, water boils at 100 degrees Celsius."
    },
    {
        "Which planet is known as the Red Planet?",
        {"Mars", "Jupiter", "Neptune", "Uranus"},
        0,
        CATEGORY_SCIENCE,
        "Mars is often called the Red Planet because of iron oxide on its surface."
    },
    {
        "What type of energy is stored in food?",
        {"Thermal energy", "Chemical energy", "Nuclear energy", "Electrical energy"},
        1,
        CATEGORY_SCIENCE,
        "Food stores chemical energy that the body converts into usable energy."
    },
    {
        "Which blood cells help fight infections?",
        {"Red blood cells", "Platelets", "White blood cells", "Plasma cells"},
        2,
        CATEGORY_SCIENCE,
        "White blood cells are key players in the immune system."
    },
    {
        "What device is used to measure temperature?",
        {"Barometer", "Thermometer", "Ammeter", "Altimeter"},
        1,
        CATEGORY_SCIENCE,
        "A thermometer measures temperature."
    },
    {
        "What is the center of our solar system?",
        {"The Moon", "The Earth", "The Sun", "Mars"},
        2,
        CATEGORY_SCIENCE,
        "The Sun is at the center of our solar system."
    }
};

const char *category_to_string(Category category) {
    switch (category) {
        case CATEGORY_C_PROGRAMMING:
            return "C Programming";
        case CATEGORY_GENERAL_KNOWLEDGE:
            return "General Knowledge";
        case CATEGORY_SCIENCE:
            return "Science";
        case CATEGORY_COUNT:
        default:
            return "Unknown";
    }
}

const Question *get_all_questions(size_t *count) {
    if (count != 0) {
        *count = sizeof(QUESTIONS) / sizeof(QUESTIONS[0]);
    }
    return QUESTIONS;
}

const Question *get_questions_by_category(Category category, size_t *count) {
    static Question filtered[sizeof(QUESTIONS) / sizeof(QUESTIONS[0])];
    size_t total = sizeof(QUESTIONS) / sizeof(QUESTIONS[0]);
    size_t index = 0;

    for (size_t i = 0; i < total; ++i) {
        if (QUESTIONS[i].category == category) {
            filtered[index++] = QUESTIONS[i];
        }
    }

    if (count != 0) {
        *count = index;
    }
    return filtered;
}
