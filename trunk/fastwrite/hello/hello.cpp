#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <fstream>
#include <string>


using namespace std;

#define DEFAULT_XPOS	200
#define DEFAULT_YPOS	200

#define DEFAULT_WIDTH	300
#define DEFAULT_HEIGHT	200

#define TEXTFILE        "save.txt"
#define CONFFILE        "config.xml"

gint XPos = DEFAULT_XPOS;
gint YPos = DEFAULT_YPOS;
gint Width = DEFAULT_WIDTH;
gint Height = DEFAULT_HEIGHT;



// =========================================================================================================
// Load window parameters from config
void  load_config()
{
        ifstream in(CONFFILE);
        string str;

        getline(in, str);  XPos = atoi(str.c_str());
        getline(in, str);  YPos = atoi(str.c_str());
        getline(in, str);  Width = atoi(str.c_str());
        getline(in, str);  Height = atoi(str.c_str());

        g_print("%d %d %d %d", XPos, YPos, Width, Height);
}


// =========================================================================================================
// Save window parameters into config
void save_config()
{
        ofstream out(CONFFILE);

        out << XPos << endl;
        out << YPos << endl;
        out << Width << endl;
        out << Height << endl;
}
// =========================================================================================================
// Get pixmap of the background (root) window
GdkPixmap* get_root_pixmap (void)
{
	GdkAtom actual_property_type = 0;
	gint actual_format = 0;
	gint actual_length = 0;
	guint* data = 0;
	gboolean res = FALSE;

	// Get the pointer to the root window
	res = gdk_property_get (
			gdk_screen_get_root_window (gdk_screen_get_default()),
			gdk_atom_intern ("_XROOTPMAP_ID", TRUE),// 	the property to retrieve. 
			GDK_TARGET_PIXMAP, // 	the desired property type
			0,
			G_MAXLONG,
			FALSE,
			&actual_property_type,
			&actual_format,
			&actual_length,
			(guchar**)&data);

	if(!res) {	
		g_print("gdk_property_get failed");
		return 0;
	}

	GdkPixmap *pixmap = gdk_pixmap_foreign_new_for_display (gdk_display_get_default(), data[0]);
	if (!pixmap) {
		g_print("gdk_pixmap_foreign_new_for_display failed");
		return 0;
	}

	gdk_drawable_set_colormap (pixmap, gdk_colormap_get_system ());
	g_free (data);

	return pixmap;
}

// =========================================================================================================
// Obtain text from textview
gchar* get_text_of(GtkTextView* textview)
{
        GtkTextBuffer* buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview));

        GtkTextIter start;
        GtkTextIter end;
        gtk_text_buffer_get_start_iter (buffer, &start);
        gtk_text_buffer_get_end_iter (buffer, &end);

        return gtk_text_buffer_get_text (buffer, &start, &end, FALSE);
}

// =========================================================================================================
// Redraw background and the text from edit control
gboolean on_expose_window (GtkWidget *widget, GdkEventExpose *event, gpointer textview) 
{
	// Draw background
	GdkWindow* gdk_window = GTK_WIDGET(widget)->window;

	GdkGC* gc = gdk_gc_new(gdk_window);
	if(!gc) {
		g_print("ERROR: gdk_gc_new failed\n");
		return FALSE;
	}

        gtk_window_get_position(GTK_WINDOW(widget), &XPos, &YPos);

	gdk_draw_drawable (gdk_window,
			   gc,
		           get_root_pixmap(),
		           XPos,YPos, // src position
		           0,0, // dst position
			   DEFAULT_WIDTH,DEFAULT_HEIGHT);

	// Draw text
	PangoLayout* text_layout = gtk_widget_create_pango_layout (widget, get_text_of(GTK_TEXT_VIEW (textview)));

	//PangoRectangle link, logical;
       // pango_layout_get_pixel_extents(text_layout, &link, &logical);
        pango_layout_set_alignment(text_layout, PANGO_ALIGN_LEFT);

        gdk_draw_layout(widget->window,
                        widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
                        0, 0,
                        text_layout);


	return TRUE;
}

// =========================================================================================================
// Save text and close the application
void on_close_window (GtkWidget *widget, GdkEventExpose *event, gpointer textview) 
{
        ofstream out(TEXTFILE);
        out << get_text_of(GTK_TEXT_VIEW (textview));

        save_config();

	gtk_main_quit ();
}


// =========================================================================================================
// Hide edit control
void on_focus_out_window (GtkWidget *widget, GdkEventExpose *event, gpointer textview) 
{
	gtk_widget_hide(GTK_WIDGET (textview));
 //       gtk_window_set_decorated(GTK_WINDOW(widget), FALSE);
}

// =========================================================================================================
// Show edit control
void on_focus_in_window (GtkWidget *widget, GdkEventExpose *event, gpointer textview) 
{
	gtk_widget_show(GTK_WIDGET  (textview));
        gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW (textview), TRUE);
//        gtk_window_set_decorated(GTK_WINDOW(widget), TRUE);
}




// =========================================================================================================
int main(int argc,  char *argv[])
{
	gtk_init (&argc, &argv);
     
        load_config();

	// Main window
	GtkWidget* window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request (window, Width, Height);
	gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
	gtk_window_move(GTK_WINDOW(window), XPos, YPos);

	// Text edit view
	GtkWidget* textview = gtk_text_view_new();
        GtkWrapMode wrap = GTK_WRAP_WORD; // see pango_get_log_attrs()!
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW (textview), wrap);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW (textview), TRUE);
	gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET(textview));

        // Read the saved text if exists, and populate the text edit with it
        ifstream in(TEXTFILE);
        string text;
        string s;
        while(getline(in, s)) {
                text += s;
                text += "\n";
        }
        
        GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (textview));
        gtk_text_buffer_set_text(buffer, text.c_str(), text.length() );



	// Events
	gtk_signal_connect (GTK_OBJECT (window), "expose_event",    GTK_SIGNAL_FUNC (on_expose_window), textview);
	gtk_signal_connect (GTK_OBJECT (window), "delete_event",    GTK_SIGNAL_FUNC (on_close_window), textview);
	gtk_signal_connect (GTK_OBJECT (window), "focus_out_event", GTK_SIGNAL_FUNC (on_focus_out_window), textview);
	gtk_signal_connect (GTK_OBJECT (window), "focus_in_event",  GTK_SIGNAL_FUNC (on_focus_in_window), textview);
	 

	gtk_widget_show_all (window);
	gtk_main ();

	return 0;
}

