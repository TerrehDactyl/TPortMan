#include "gtktemplate.h"

// compile with gcc -Wall -g tportman.c -o tportman `pkg-config --cflags --libs gtk+-3.0`

void packboxes(int i);
void get_entry_text(GtkWidget *widget[], const gchar *entries[], size_t entrysize);
void createnotebookpage(int i, GtkWidget *notebook, gchar *buttonlabels[], void *buttoncallbacks[], size_t butt_size, size_t chooserarr_size, gchar *labeltext[]);
void cmpinterest();
void on_changed(GtkComboBox *combo_box, gpointer user_data);

struct widgets
{
	GtkWidget *buttonbox;
	GtkWidget *frame;
	GtkWidget *cmpinterest_entry_grid;
	GtkWidget *cmpinterest_entry_label_grid;
	GtkWidget *cmpinterest_entries[3];
	GtkComboBox *combo_box;
	GtkWidget *combo_label_grid;
	GtkWidget *display; //widget for the display 
	GtkTextBuffer *buffer; //widget for the text
}gwidget;

struct variables
{
	const gchar *cmpinterest_entries[9];
	double cmp_freq;
}location;

gchar *cmpinterest_entry_labels[] = {"Principle Investment\n", "Interest Rate\n", "Term (in Months)\n"};
gchar *combo_headers[] = {"	Compound Rate\n"};
gchar *combo_labels[] = {"","Daily", "Weekly", "Monthly", "Annually"};
gchar *pageheads[] = {"Compound Interest"};
size_t combo_size = arraysize(combo_labels);
size_t combo_header_size = arraysize(combo_headers);
size_t cmpinterest_entry_size = arraysize(cmpinterest_entry_labels);

int main(int argc, char *argv [])
{
gchar *cmpinterest_labels[] = {"Principle\n", "Interest Rate\n"};
gchar *cmpinterest_butt_labels[]= {"Calculate"}; 
void *cmpinterest_butt_callbacks[] = {cmpinterest}; //i do this to make it easier to build onto the code.
size_t cmpinterest_size = arraysize(cmpinterest_labels);
size_t cmpinterest_butt_size = arraysize(cmpinterest_butt_labels);

gtk_init(&argc, &argv); //starting gtk 

GtkWidget *window = createwindow("tportman", GTK_WIN_POS_CENTER);
GtkWidget *notebook = createnotebook(window);
createnotebookpage(0, notebook, cmpinterest_butt_labels, cmpinterest_butt_callbacks, cmpinterest_butt_size, cmpinterest_size, cmpinterest_labels);

show_and_destroy(window); //shows all widgets, connects the callback for the window and starts gtkmain
}

void createnotebookpage(int i, GtkWidget *notebook, gchar *buttonlabels[], void *buttoncallbacks[], size_t butt_size, size_t chooserarr_size, gchar *labeltext[])
{
	gwidget.buttonbox = createsinglesizegrid(buttonlabels, buttoncallbacks, 1, butt_size);
	gwidget.frame = create_frame_with_pagehead(notebook, pageheads, i);
	gwidget.cmpinterest_entry_grid = gtk_grid_new();
	gwidget.combo_box = create_combobox(combo_labels,  combo_size, on_changed);
	gwidget.display = create_text_display(gwidget.display, TRUE, 150, 40);
	gwidget.buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW ( gwidget.display)); //sets the gwidget.buffer widget to the text gwidget.buffer of display
	create_entries(cmpinterest_entry_size, gwidget.cmpinterest_entries, gwidget.cmpinterest_entry_grid);
	gwidget.combo_label_grid = createlabels(combo_headers, combo_header_size);
	gwidget.cmpinterest_entry_label_grid = createlabels(cmpinterest_entry_labels, cmpinterest_entry_size);
	packboxes(i);
}

void packboxes(int i) //i use a switch to make it easy to build onto this code
{
	GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1); //creates a vertical box without autosizing 1
	GtkWidget *entrybox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1); //creates a horizontal box without autosizing 
	GtkWidget *combobox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1); //creates a vertical box without autosizing
	GtkWidget *inner_combo =  gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);

	gtk_container_add (GTK_CONTAINER (gwidget.frame), vbox);
	gtk_box_pack_start(GTK_BOX(vbox),  entrybox, FALSE, FALSE, 0); 
	gtk_box_pack_start(GTK_BOX(entrybox),  gwidget.cmpinterest_entry_label_grid, FALSE, FALSE, 0); 
	gtk_box_pack_start(GTK_BOX(entrybox),  gwidget.cmpinterest_entry_grid, FALSE, FALSE, 0); //attaches the entries 
	gtk_box_pack_start(GTK_BOX(vbox),  combobox, FALSE, FALSE, 0); 
	gtk_box_pack_start(GTK_BOX(combobox),  gwidget.combo_label_grid, FALSE, FALSE, 0); //attaches the label grid
	gtk_box_pack_start(GTK_BOX(combobox),  inner_combo, FALSE, FALSE, 0); //
	gtk_box_pack_start(GTK_BOX(inner_combo),  GTK_WIDGET(gwidget.combo_box), FALSE, FALSE, 0); //Attaches the combo box
	gtk_box_pack_start(GTK_BOX(vbox),  gwidget.buttonbox, FALSE, FALSE, 0); //Attaches the buttons
	gtk_box_pack_start(GTK_BOX(vbox),  gwidget.display, FALSE, FALSE, 0); //Attaches the display
}

void on_changed(GtkComboBox *combo_box, gpointer user_data)
{
	if (gtk_combo_box_get_active (combo_box) != 0) 
	{
		gchar *selection = gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(combo_box));
		if(strcmp(selection, "Daily") == 0)
		{
			location.cmp_freq = 30.416666667;
		}
		if(strcmp(selection, "Weekly") == 0)
		{
			location.cmp_freq = 4.3334;
		}
		if(strcmp(selection, "Monthly") == 0)
		{
			location.cmp_freq = 1;
		}
		if(strcmp(selection, "Annually") == 0)
		{
			location.cmp_freq = 0;
		}
	}
}

void cmpinterest()
{
	get_entry_text(gwidget.cmpinterest_entries, location.cmpinterest_entries, cmpinterest_entry_size);

	long double principle = strtold(location.cmpinterest_entries[0], NULL);
	long double interest = strtold(location.cmpinterest_entries[1], NULL); 
	long double temp;
	int term = strtold(location.cmpinterest_entries[2], NULL);
	char preview[128];
	int cmp_times;
	interest/=100;

	if(location.cmp_freq == 0)
	{
		cmp_times = term/12;
	}
	else
	{
		cmp_times = term*location.cmp_freq;
	}
	for(int i = 0; i < cmp_times; i++)
	{
		temp = principle*interest;
		principle += temp;
	}
sprintf(preview,"%Lf", principle); //casts the double into a char array to be used for the buffer 
gtk_text_buffer_set_text ( gwidget.buffer, preview, -1);
}
