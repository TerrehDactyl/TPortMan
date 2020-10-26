#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#define arraysize(x)  (sizeof(x) / sizeof((x)[0]))

// compile with gcc -Wall -g -lm tportman.c -o tportman `pkg-config --cflags --libs gtk+-3.0`

void createnotebook(GtkWidget *window);
GtkWidget *createwindow(char * title, GtkWindowPosition position);
void packboxes(int i);
void createlabels(GtkWidget *grid, gchar *labeltext[], size_t arraylen);
void createsinglesizegrid(GtkWidget *grid, gchar *labels[], void *callback[], int rows, int columns);
void button_connect_callback(GtkWidget *button, void *button_callback, gpointer data);
void createradiobuttons(GtkWidget *radiobox, gchar *radiolabels[], void *radiocallback[], int arraysize);
void set_spacing(GtkWidget *widget, int colspace, int rowspace);
void create_entries(int entry_len, GtkWidget *entries[], GtkWidget *entrygrid);
void get_entry_text(GtkWidget *widget[], const gchar *entries[], size_t entrysize);
void createnotebookpage(int i, gchar *buttonlabels[], void *buttoncallbacks[], size_t butt_size, size_t chooserarr_size, gchar *labeltext[]);
void cmpinterest();
void create_combobox();
void on_changed(GtkWidget *widget, gpointer user_data);
void create_combobox(GtkWidget *combo_box, gchar *combo_labels[], size_t combo_size, void *callback);
void get_entry_text(GtkWidget *widget[], const gchar *entries[], size_t entrysize);
void term_changed(GtkWidget *widget, gpointer user_data);

struct widgets
{
	GtkWidget *vbox; //widget for the vertical box
	GtkWidget *hbox;
	GtkWidget *buttonbox;
	GtkWidget *radiobox;
	GtkWidget *rootbutton;
	GtkWidget *chooservbox;
	GtkWidget *notebook;
	GtkWidget *frame;
	GtkWidget *table;
	GtkWidget *framebox;
	GtkWidget *entrygrid;
	GtkWidget *entries[9];
	GtkWidget *entrylabelgrid;
	GtkWidget *labelgrid;
	GtkWidget *cmpinterest_entry_grid;
	GtkWidget *cmpinterest_entry_label_grid;
	GtkWidget *cmpinterest_entries[3];
    GtkWidget *text_entry;
    GtkWidget *combo_box;
    GtkWidget *combo_label_grid;
    GtkWidget *term_combo_box;
    GtkWidget *display; //widget for the display 
	GtkTextBuffer *buffer; //widget for the text
}gwidget;

struct variables
{
	unsigned int algorithm;
	const gchar *rebalancer_entries[3];
	const gchar *cmpinterest_entries[9];
	int placement;
	double cmp_freq;
}location;

gchar *cmpinterest_labels[] = {"Principle\n", "Interest Rate\n"};
gchar *cmpinterest_butt_labels[]= {"Calculate"}; 
gchar *cmpinterest_entry_labels[] = {"Principle Investment\n", "Interest Rate\n", "Term\n"};
gchar *combo_headers[] = {"	Compound Rate\n"};
gchar *combo_labels[] = {"","Daily", "Weekly", "Monthly", "Annually"};
void *cmpinterest_butt_callbacks[] = {cmpinterest};
size_t cmpinterest_size = arraysize(cmpinterest_labels);
size_t cmpinterest_butt_size = arraysize(cmpinterest_butt_labels);
size_t cmpinterest_entry_size = arraysize(cmpinterest_entry_labels);
size_t combo_size = arraysize(combo_labels);
size_t combo_header_size = arraysize(combo_headers);

gchar *pageheads[] = {"Compound Interest"};

int main(int argc, char *argv [])
{
	gtk_init(&argc, &argv); //starting gtk 

	GtkWidget *window = createwindow("tportman", GTK_WIN_POS_CENTER);
	createnotebook(window);
	createnotebookpage(0, cmpinterest_butt_labels, cmpinterest_butt_callbacks, cmpinterest_butt_size, cmpinterest_size, cmpinterest_labels);

	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_widget_show_all(window); //shows all widgets 

	gtk_main();//gtk main, this is the main loop of GTK
}

void packboxes(int i)
{
	gwidget.vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1); //creates a vbox without autosizing 
	gwidget.hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1); //creates a vbox without autosizing 
	GtkWidget *entrybox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1); //creates a vbox without autosizing 
	GtkWidget *combobox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1); //creates a vbox without autosizing
	GtkWidget *inner_combo =  gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
	switch(i)
	{
		case 0:
		gtk_container_add (GTK_CONTAINER (gwidget.frame), gwidget.vbox);
		gtk_box_pack_start(GTK_BOX(gwidget.vbox),  gwidget.hbox, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(gwidget.vbox),  entrybox, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(entrybox),  gwidget.cmpinterest_entry_label_grid, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(entrybox),  gwidget.cmpinterest_entry_grid, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(gwidget.vbox),  combobox, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(combobox),  gwidget.combo_label_grid, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(combobox),  inner_combo, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(inner_combo),  gwidget.combo_box, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(gwidget.vbox),  gwidget.buttonbox, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(gwidget.vbox),  gwidget.display, FALSE, FALSE, 0); //packs the display into the vbox
		break;

		default:
		break;
	}
}

GtkWidget *createwindow(char * title, GtkWindowPosition position)
{
	GtkWidget *widget = gtk_window_new(GTK_WINDOW_TOPLEVEL); //creates toplevel window
	gtk_window_set_title(GTK_WINDOW(widget), title); //sets a window title 
	gtk_window_set_position(GTK_WINDOW(widget), position); //opens the window in the center of the screen
	gtk_container_set_border_width(GTK_CONTAINER(widget), 5); //sets the border size of the window
	return widget;
}

void createnotebook(GtkWidget *window)
{
	gwidget.table = gtk_grid_new ();
	gwidget.notebook = gtk_notebook_new ();
	gtk_grid_attach(GTK_GRID(gwidget.table), gwidget.notebook, 0, 1, 1, 1); 
	gtk_container_add (GTK_CONTAINER (window), gwidget.table);
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (gwidget.notebook), GTK_POS_TOP);
}

void createnotebookpage(int i, gchar *buttonlabels[], void *buttoncallbacks[], size_t butt_size, size_t chooserarr_size, gchar *labeltext[])
{
	gwidget.buttonbox = gtk_grid_new(); 
	gwidget.radiobox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
	gwidget.frame = gtk_frame_new (pageheads[i]);
	GtkWidget *pagelabel = gtk_label_new(pageheads[i]);
	gwidget.labelgrid = gtk_grid_new();

    gtk_container_set_border_width (GTK_CONTAINER (gwidget.frame), 10);
    gtk_widget_set_size_request (gwidget.frame, 100, 75);

    gtk_notebook_append_page (GTK_NOTEBOOK (gwidget.notebook), gwidget.frame, pagelabel);
    createlabels(gwidget.labelgrid, labeltext, chooserarr_size);
    createsinglesizegrid(gwidget.buttonbox, buttonlabels, buttoncallbacks, 1, butt_size);


	switch(i)
	{
		case 0:
		gwidget.cmpinterest_entry_grid = gtk_grid_new();
		gwidget.cmpinterest_entry_label_grid = gtk_grid_new();
		gwidget.combo_label_grid = gtk_grid_new();
		gwidget.combo_box = gtk_combo_box_text_new ();
		gwidget.display = gtk_text_view_new (); //sets the display widget as a text display 
		gwidget.buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW ( gwidget.display)); //sets the gwidget.buffer widget to the text gwidget.buffer of display
		gtk_text_view_set_editable (gwidget.display,FALSE);
		gtk_widget_set_size_request( gwidget.display, 150, 40); //sets the size of the display 


		create_entries(cmpinterest_entry_size, gwidget.cmpinterest_entries, gwidget.cmpinterest_entry_grid);
		create_combobox(gwidget.combo_box, combo_labels,  combo_size, on_changed);
		createlabels(gwidget.combo_label_grid, combo_headers, combo_header_size);
		createlabels(gwidget.cmpinterest_entry_label_grid, cmpinterest_entry_labels, cmpinterest_entry_size);
		packboxes(i);
		break;

		default: 
		packboxes(i);
		break;
	}
}

void create_entries(int entry_len, GtkWidget *entries[], GtkWidget *entrygrid)
{
	for(int i = 0; i < entry_len; i++)
	{
	entries[i]= gtk_entry_new();
	gtk_grid_attach(GTK_GRID(entrygrid), entries[i], 0, i, 1, 1); //sets the defaults for creating each table button
	}
	set_spacing(entrygrid, 4, 4);
}

void create_combobox(GtkWidget *combo_box, gchar *combo_labels[], size_t combo_size, void *callback)
{

	for (int i = 0; i < combo_size; i++)
	{
		gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo_box), combo_labels[i]);
	}
	gtk_combo_box_set_active (GTK_COMBO_BOX (combo_box), 0);
	g_signal_connect (combo_box,"changed",G_CALLBACK (callback),NULL);
}

void on_changed(GtkWidget *widget, gpointer user_data)
{
	GtkComboBox *combo_box = widget;

  if (gtk_combo_box_get_active (combo_box) != 0) 
  {
    gchar *selection = gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(combo_box));
    g_print("%s\n", selection);
    if(strcmp(selection, "Daily") == 0)
    {
    	location.cmp_freq = 30.42;
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

void createlabels(GtkWidget *grid, gchar *labeltext[], size_t arraylen)
{
	for (int i=0; i<arraylen; i++)
	{
		for(int j = 0; j<1; j++)
		{	
			GtkWidget *label = gtk_label_new(labeltext[i]);
			gtk_grid_attach(GTK_GRID(grid), label, j, i, 1, 1); //sets the defaults for creating each table button
		}
	}
}

void createradiobuttons(GtkWidget *radiobox, gchar *radiolabels[], void *radiocallback[], int arraysize)
{
	GtkWidget *rootbutton = gtk_radio_button_new_with_label(NULL, radiolabels[0]);
	button_connect_callback(rootbutton, radiocallback[0], NULL);
	location.placement++;
	gtk_box_pack_start(GTK_BOX(radiobox), rootbutton, FALSE, FALSE, 0); //packs the display into the vbox

	for (int i = 1; i<arraysize; i++)
	{
	GtkWidget *labels = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(rootbutton), radiolabels[i]);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(labels), FALSE);
	button_connect_callback(labels, radiocallback[i], NULL);
	location.placement++;
	gtk_box_pack_start(GTK_BOX(radiobox), labels, FALSE, FALSE, 0); //packs the display into the vbox
	}
}

void createsinglesizegrid(GtkWidget *grid, gchar *labels[], void *callback[], int rows, int columns)
{
	int pos = 0;

	for (int i=0; i < rows; i++) //for loop for the rows
	{
		for (int j=0; j < columns; j++) //for loop for the columns
		{
		GtkWidget *button = gtk_button_new_with_label(labels[pos]); //sets each button label to the respective button 
		gpointer pointer = (void *) (long) location.placement;
		button_connect_callback(button, callback[pos],pointer); //attaches the button to the respective callback
		location.placement++;
		gtk_grid_attach(GTK_GRID(grid), button, j, i, 1, 1); //sets the defaults for creating each table button
		gtk_widget_set_size_request(button, 70, 30); //sets the size of the buttons
		pos++; //changes the position 
		}
		set_spacing(grid, 4, 4);
	}
}

void button_connect_callback(GtkWidget *button, void *button_callback, gpointer data)
{
	g_signal_connect(button, "clicked", G_CALLBACK(button_callback), data);
}

void set_spacing(GtkWidget *widget, int colspace, int rowspace)
{
	gtk_grid_set_column_spacing(GTK_GRID(widget), colspace);
	gtk_grid_set_row_spacing(GTK_GRID(widget), rowspace);
}

void get_entry_text(GtkWidget *widget[], const gchar *entries[], size_t entrysize)
{
	for(int i = 0; i < entrysize; i++)
	{
		entries[i] = gtk_entry_get_text(GTK_ENTRY(widget[i]));
	}
}

void cmpinterest()
{
get_entry_text(gwidget.cmpinterest_entries, location.cmpinterest_entries, cmpinterest_entry_size);

double principle = strtol(location.cmpinterest_entries[0], NULL, 10);
double temp;
double interest = strtol(location.cmpinterest_entries[1], NULL, 10); 
int term = strtol(location.cmpinterest_entries[2], NULL, 10);
interest/=100;

char preview[128];
int cmp_times;

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
    sprintf(preview,"%f", principle); //copies input.num1 to the preview 
	gtk_text_buffer_set_text ( gwidget.buffer, preview, -1);
}