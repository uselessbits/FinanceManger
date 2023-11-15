# FinanceManger
FinanceManager project in C
Vela Paul Valentin 217, Info RO

# Overview

This code is a simple financial manager implemented in C using the GTK library. It provides a graphical user interface (GUI) for users to manage financial transactions, including adding, deleting, saving, loading, and generating reports.
Structures


Transaction Structure
Represents a financial transaction with fields for date, amount, description, and type.

    typedef struct {
        gchar date[12];
        gchar amount[10];
        gchar description[30];
        gchar type[3];
    } transaction;

  
Global Variables

  transactionArray[100]: An array to store financial transactions.
  numEntries: Keeps track of the number of entries in transactionArray.
  view: A GTK widget representing the main tree view for displaying transactions.
  label: A GTK widget representing a label for displaying the current balance.

    
    enum {
        COL_DATE = 0,
        COL_AMOUNT,
        COL_DESCRIPTION,
        COL_TYPE,
        NUM_COLS
    };

Enumerates the columns of the tree view model.

# Functions
    create_and_fill_model(transaction data[])

Creates and fills a GTK tree model with financial transaction data.
Used to initialize the tree view.

    create_view_and_model(GtkTreeModel *model)

Creates a GTK tree view and sets its model.
Configures columns for the tree view.

    stringToDouble(const char *str)

Converts a string to a double, handling errors.

getType(const char *str)

Converts a transaction type string to an integer (1 for income, -1 for expense).

    calculate_sum()

Calculates the balance based on the transaction array.

    isValidDate(const char *str)

Checks if a date string is in a valid format.

    deleteEntry(const char *str)

Deletes a transaction entry based on the given index.
Updates the balance label and tree view after deletion.

    delete(GtkWindow *window)

Displays a dialog for entering the index of the entry to delete.

    create_input_dialog(GtkWindow *window)

Displays a dialog for entering a new financial transaction.
Validates input and updates the transaction array, balance, label, and tree view.

    convertDateFormat(const char *inputDate, char *outputDate)

Converts the date format for comparisons.

    isInInterval(const char *date, const char *left, const char *right)

Checks if a date is within a specified interval.

    printtransactionArray()

Prints the contents of the transaction array for debugging.

    add_transaction(GtkButton *button, gpointer user_data)

Callback function for the "Add Transaction" button.

    createRaportFiles(const char *str1, const char *str2)

Creates a report file containing transactions within a specified date interval.

    create_filter_dialog(GtkWindow *window)

Displays a dialog for entering date filters for generating a report.

    createRaport(GtkButton *button, gpointer user_data)

Callback function for the "Create Raport" button.

    saveDataToFile()

Saves all entries in the transaction array to a text document.

    loadDataFromFile()

Loads entries from a text document into the transaction array.

    main(int argc, char **argv)

The main function initializing the GTK window and handling GUI events.

# User Interface

    The GUI includes buttons for adding transactions, saving/loading data, creating reports, and deleting entries.
    The main view is a tree view displaying transaction details.
    The current balance is displayed as a label.
