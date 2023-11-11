#include <gtk/gtk.h>

typedef struct {
    gchar date[12];
    gchar amount[10];
    gchar description[30];
    gchar type[3];
} transaction;


transaction transactionArray[100]; // Array to store transactions
int numEntries = 0; // Keep track of the number of entries
static GtkWidget *view;
static GtkWidget *label ;


enum
{
    COL_DATE = 0,
    COL_AMOUNT,
    COL_DESCRIPTION,
    COL_TYPE,
    NUM_COLS
} ;

static GtkTreeModel * create_and_fill_model(transaction data[])
{
    GtkTreeStore *treestore;
    GtkTreeIter iter;

    treestore = gtk_tree_store_new(NUM_COLS,
                                   G_TYPE_STRING,
                                   G_TYPE_STRING,
                                   G_TYPE_STRING,
                                   G_TYPE_STRING);

    for (int i = 0; i < numEntries; i++) {
        gtk_tree_store_append(treestore, &iter, NULL);
        gtk_tree_store_set(treestore, &iter,
                           COL_DATE, data[i].date,
                           COL_AMOUNT, data[i].amount,
                           COL_DESCRIPTION, data[i].description,
                           COL_TYPE, data[i].type,
                           -1);
    }

    return GTK_TREE_MODEL(treestore);
}

static GtkWidget * create_view_and_model (GtkTreeModel *model)
{
    GtkTreeViewColumn   *col;
    GtkCellRenderer     *renderer;



    view = gtk_tree_view_new();

    /* Column #1  */

    col = gtk_tree_view_column_new();
    gtk_tree_view_column_set_title(col, "Date");
    gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
    renderer = gtk_cell_renderer_text_new();

    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_add_attribute(col, renderer, "text", COL_DATE);


    /* Column #2 */

    col = gtk_tree_view_column_new();
    gtk_tree_view_column_set_title(col, "Amount");
    gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
    renderer = gtk_cell_renderer_text_new();

    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_add_attribute(col, renderer, "text", COL_AMOUNT);


    /* Column #3 */

    col = gtk_tree_view_column_new();
    gtk_tree_view_column_set_title(col, "Description");
    gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
    renderer = gtk_cell_renderer_text_new();


    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_add_attribute(col, renderer, "text", COL_DESCRIPTION);

    /* --- Column #4 --- */

    col = gtk_tree_view_column_new();
    gtk_tree_view_column_set_title(col, "Type");
    gtk_tree_view_append_column(GTK_TREE_VIEW(view),col);
    renderer = gtk_cell_renderer_text_new();

    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_add_attribute(col, renderer, "text", COL_TYPE);




    gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
    g_object_unref(model);

    gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(view)),
                                GTK_SELECTION_NONE);



    return view;
}

double stringToDouble(const char *str) {
    char *endptr;
    double result = strtod(str, &endptr);

    // Check for errors during conversion
    if (*endptr != '\0') {

        fprintf(stderr, "Error converting string to double: %s\n", str);
        return 0;
    }

    return result;
}

int getType(const char *str)
{
    if(strcmp(str, "I")==0)return 1;
    if(strcmp(str, "E")==0)return -1;
    printf("Incorrect type");
    return 0;

}


double calculate_sum()
{
    double balance = 0;
    for (int i = 0; i < numEntries; i++) {
        double one = getType(transactionArray[i].type);

        balance = balance + one* stringToDouble(transactionArray[i].amount);
    }
    return balance;
}

int isValidDate(const char *str)
{
    int day, month, year;
    if (sscanf(str, "%d-%d-%d", &day, &month, &year) != 3) {
        printf("Data Invalida");// Failed to parse three integers
    }
    if(day>31 || day<=0 || month>12 || month<=0 )
        return 0;
    return 1;

}
void deleteEntry(const char *str)
{

    char *endptr;

    long index = strtol(str, &endptr, 10);

    if (*endptr != '\0') {
        printf("Conversion error\n", str);
    }
    else{
        for(int i = index; i < numEntries - 1; i++) transactionArray[i] = transactionArray[i + 1];
        numEntries--;
        double balance = calculate_sum();

        char str2[30] = "Balance: ";
        if (balance < 0) {
            sprintf(str2, "Balance: -%.2lf", -balance);
        } else {
            sprintf(str2, "Balance: %.2lf", balance);
        }


        gtk_label_set_text(GTK_LABEL(label), str2 );

        GtkTreeModel *newModel = create_and_fill_model(transactionArray);
        gtk_tree_view_set_model(GTK_TREE_VIEW(view), newModel);

    }

}

void delete(GtkWindow *window){


    GtkWidget *dialog = gtk_dialog_new_with_buttons("Input Dialog", window, GTK_DIALOG_MODAL,
                                                    "OK", GTK_RESPONSE_ACCEPT, "Cancel", GTK_RESPONSE_CANCEL, NULL);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *entry1 = gtk_entry_new();
    GtkWidget *label1 = gtk_label_new("Delete entry(0-indexed):");
    gtk_entry_set_input_purpose(GTK_ENTRY(entry1), GTK_INPUT_PURPOSE_NUMBER);
    gtk_container_add(GTK_CONTAINER(content_area), label1);
    gtk_container_add(GTK_CONTAINER(content_area), entry1);

    gtk_widget_show_all(dialog);

    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    deleteEntry(gtk_entry_get_text(GTK_ENTRY(entry1)));
    gtk_widget_destroy(dialog);
}

// Function to create and show an input dialog with a numeric input field
void create_input_dialog(GtkWindow *window) {
    transaction data;
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Input Dialog", window, GTK_DIALOG_MODAL,
                                                    "OK", GTK_RESPONSE_ACCEPT, "Cancel", GTK_RESPONSE_CANCEL, NULL);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    GtkWidget *entry1 = gtk_entry_new();
    GtkWidget *entry2 = gtk_entry_new();
    GtkWidget *entry3 = gtk_entry_new();
    GtkWidget *entry4 = gtk_entry_new();

    // Create labels for the fields
    GtkWidget *label1 = gtk_label_new("Date:");
    GtkWidget *label2 = gtk_label_new("Amount:");
    GtkWidget *label3 = gtk_label_new("Description:");
    GtkWidget *label4 = gtk_label_new("Type:");

    gtk_entry_set_input_purpose(GTK_ENTRY(entry1), GTK_INPUT_PURPOSE_ALPHA);
    gtk_entry_set_input_purpose(GTK_ENTRY(entry2), GTK_INPUT_PURPOSE_NUMBER);
    gtk_entry_set_input_purpose(GTK_ENTRY(entry3), GTK_INPUT_PURPOSE_ALPHA);
    gtk_entry_set_input_purpose(GTK_ENTRY(entry4), GTK_INPUT_PURPOSE_ALPHA);


    // Pack the labels and entry widgets into the dialog
    gtk_container_add(GTK_CONTAINER(content_area), label1);
    gtk_container_add(GTK_CONTAINER(content_area), entry1);
    gtk_container_add(GTK_CONTAINER(content_area), label2);
    gtk_container_add(GTK_CONTAINER(content_area), entry2);
    gtk_container_add(GTK_CONTAINER(content_area), label3);
    gtk_container_add(GTK_CONTAINER(content_area), entry3);
    gtk_container_add(GTK_CONTAINER(content_area), label4);
    gtk_container_add(GTK_CONTAINER(content_area), entry4);

    gtk_widget_show_all(dialog);

    gint result = gtk_dialog_run(GTK_DIALOG(dialog));

    if (result == GTK_RESPONSE_ACCEPT && stringToDouble(gtk_entry_get_text(GTK_ENTRY(entry2))) &&
            (getType(gtk_entry_get_text(GTK_ENTRY(entry4)))==1 || getType(gtk_entry_get_text(GTK_ENTRY(entry4)))==-1) &&
            isValidDate(gtk_entry_get_text(GTK_ENTRY(entry1)))
    )
    {

            // Get the text from the entry widgets and store it in the struct
        strcpy(data.date,gtk_entry_get_text(GTK_ENTRY(entry1)));
        strcpy(data.amount,gtk_entry_get_text(GTK_ENTRY(entry2)));
        strcpy(data.description,gtk_entry_get_text(GTK_ENTRY(entry3)));
        strcpy(data.type,gtk_entry_get_text(GTK_ENTRY(entry4)));


        transactionArray[numEntries] = data;
        numEntries++;

    }


    gtk_widget_destroy(dialog);
}


void convertDateFormat(const char *inputDate, char *outputDate) {
    int day, month, year;
    sscanf(inputDate, "%d-%d-%d", &day, &month, &year);
    snprintf(outputDate, sizeof(outputDate), "%04d-%02d-%02d", year, month, day);
}

int isInInterval(const char *date, const char *left, const char *right) {
    char convertedDate[11];
    char convertedLeft[11];
    char convertedRight[11];

    // Convert input dates to comparable format
    convertDateFormat(date, convertedDate);
    convertDateFormat(left, convertedLeft);
    convertDateFormat(right, convertedRight);

    // Check if the date is within the interval
    return (strcmp(convertedDate, convertedLeft) >= 0 && strcmp(convertedDate, convertedRight) <= 0);
}

void printtransactionArray() {
    for (int i = 0; i < numEntries; i++) {
        g_print("Entry %d:\n", i + 1);
        g_print("Field 1: %s\n", transactionArray[i].date);
        g_print("Field 2: %s\n", transactionArray[i].amount);
        g_print("Field 3: %s\n", transactionArray[i].description);
        g_print("Field 4: %s\n", transactionArray[i].type);
    }
}


void add_transaction(GtkButton *button, gpointer user_data) {
    GtkWidget *window = GTK_WIDGET(user_data);
    //Shows the user the input form
    create_input_dialog(GTK_WINDOW(window));

    printtransactionArray(); // Print the array after each submission, for tests

    //calculates the balanced based on the transactionsArray
    double balance = calculate_sum();

    char str2[30] = "Balance: ";
    if (balance < 0) {
        sprintf(str2, "Balance: -%.2lf", -balance);
    } else {
        sprintf(str2, "Balance: %.2lf", balance);
    }
    //Modifies the string corespondendly with the amount

    //Updates the balance label and the tree_view data
    gtk_label_set_text(GTK_LABEL(label), str2 );

    GtkTreeModel *newModel = create_and_fill_model(transactionArray);
    gtk_tree_view_set_model(GTK_TREE_VIEW(view), newModel);
}

void createRaportFiles(const char *str1, const char *str2)
{
    FILE *raport_file = fopen("financial_raport.txt", "w");
    if (raport_file == NULL) {
        perror("Error opening file");
        return;
    }
    for (int i = 0; i < numEntries; i++) {
        if(isInInterval(transactionArray[i].date,str1,str2))
        fprintf(raport_file, "%s,%s,%s,%s\n", transactionArray[i].date,
                transactionArray[i].amount, transactionArray[i].description,
                transactionArray[i].type);
    }
}




void create_filter_dialog(GtkWindow *window)
{
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Filter Dialog", window, GTK_DIALOG_MODAL,
                                                    "OK", GTK_RESPONSE_ACCEPT, "Cancel", GTK_RESPONSE_CANCEL, NULL);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *entry1 = gtk_entry_new();
    GtkWidget *entry2 = gtk_entry_new();
    GtkWidget *label1 = gtk_label_new("First Date:");
    GtkWidget *label2 = gtk_label_new("Last Date:");

    gtk_container_add(GTK_CONTAINER(content_area), label1);
    gtk_container_add(GTK_CONTAINER(content_area), entry1);
    gtk_container_add(GTK_CONTAINER(content_area), label2);
    gtk_container_add(GTK_CONTAINER(content_area), entry2);

    gtk_widget_show_all(dialog);

    gint result = gtk_dialog_run(GTK_DIALOG(dialog));

    if (result == GTK_RESPONSE_ACCEPT &&
        isValidDate(gtk_entry_get_text(GTK_ENTRY(entry1))) &&
        isValidDate(gtk_entry_get_text(GTK_ENTRY(entry2))) )

        createRaportFiles(gtk_entry_get_text(GTK_ENTRY(entry1)),gtk_entry_get_text(GTK_ENTRY(entry2)));

    gtk_widget_destroy(dialog);

}

void createRaport(GtkButton *button, gpointer user_data)
{
    GtkWidget *window = GTK_WIDGET(user_data);
    create_filter_dialog(GTK_WINDOW(window));


}
void saveDataToFile() {
    //Saves all the entries in a text document, ex 12-10-2022,1000,Transaction,I
    FILE *file = fopen("financial_data.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < numEntries; i++) {
        fprintf(file, "%s,%s,%s,%s\n", transactionArray[i].date,
                transactionArray[i].amount, transactionArray[i].description,
                transactionArray[i].type);
    }

    fclose(file);
}



void loadDataFromFile() {
    FILE *file = fopen("financial_data.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    numEntries=0;
    while (fscanf(file, "%10s,%[^,],%[^,],%1s\n", transactionArray[numEntries].date,
                  transactionArray[numEntries].amount, &transactionArray[numEntries].description,
                  &transactionArray[numEntries].type) != EOF) {
        (numEntries)++;
    }

    fclose(file);


    double balance = calculate_sum();

    char str2[30] = "Balance: ";
    if (balance < 0) {
        sprintf(str2, "Balance: -%.2lf", -balance);
    } else {
        sprintf(str2, "Balance: %.2lf", balance);
    }


    gtk_label_set_text(GTK_LABEL(label), str2 );

    GtkTreeModel *newModel = create_and_fill_model(transactionArray);
    gtk_tree_view_set_model(GTK_TREE_VIEW(view), newModel);

}




int main(int argc, char **argv) {
    gtk_init(&argc, &argv);



    GtkWidget *window;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Manager");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);//Doesn't quite work on linux, dunno why

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    view = create_view_and_model(create_and_fill_model(transactionArray));




    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), box);


    // Create three buttons
    GtkWidget *add_button = gtk_button_new_with_label("Add Transaction");
    g_signal_connect(add_button, "clicked", G_CALLBACK(add_transaction), window);
    
    GtkWidget *save_button = gtk_button_new_with_label("Save Data");
    g_signal_connect(save_button, "clicked", G_CALLBACK(saveDataToFile), window);

    GtkWidget *load_button = gtk_button_new_with_label("Load Data");
    g_signal_connect(load_button, "clicked", G_CALLBACK(loadDataFromFile), window);

    GtkWidget *raport_button = gtk_button_new_with_label("Create Raport");
    g_signal_connect(raport_button, "clicked",G_CALLBACK(createRaport),window);

    GtkWidget *delete_button = gtk_button_new_with_label("Delete");
    g_signal_connect(delete_button, "clicked",G_CALLBACK(delete),window);

    //Adds and connects the main buttons to their functions, signaled by click




    label = gtk_label_new("Amount: ");


    // Pack the buttons into the box
    gtk_box_pack_start(GTK_BOX(box), add_button,1,1,0);
    gtk_box_pack_start(GTK_BOX(box), save_button,1,1,0);
    gtk_box_pack_start(GTK_BOX(box), load_button, 1,1,0);
    gtk_box_pack_start(GTK_BOX(box), raport_button, 1,1,0);
    gtk_box_pack_start(GTK_BOX(box), delete_button, 1,1,0);
    gtk_box_pack_start(GTK_BOX(box),label, 1,1,0);
    gtk_box_pack_start(GTK_BOX(box), view,1,1,0);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}