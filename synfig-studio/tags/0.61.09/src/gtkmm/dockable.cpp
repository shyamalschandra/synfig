/* === S Y N F I G ========================================================= */
/*!	\file dockable.cpp
**	\brief Template File
**
**	$Id$
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
**
**	This package is free software; you can redistribute it and/or
**	modify it under the terms of the GNU General Public License as
**	published by the Free Software Foundation; either version 2 of
**	the License, or (at your option) any later version.
**
**	This package is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
**	General Public License for more details.
**	\endlegal
*/
/* ========================================================================= */

/* === H E A D E R S ======================================================= */

#ifdef USING_PCH
#	include "pch.h"
#else
#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include "app.h"
#include <sigc++/hide.h>

#include "dockable.h"
#include "dockmanager.h"
#include "dockbook.h"
#include "dockdialog.h"
#include <synfig/general.h>
#include <gtkmm/table.h>
#include <gtk/gtk.h>

#include "general.h"

#endif

/* === U S I N G =========================================================== */

using namespace std;
using namespace etl;
using namespace synfig;
using namespace studio;

/* === M A C R O S ========================================================= */

#ifdef WIN32
#	ifdef IMAGE_DIR
#		undef IMAGE_DIR
#		define IMAGE_DIR "share\\pixmaps"
#	endif
#endif

#ifndef IMAGE_DIR
#	define IMAGE_DIR "/usr/local/share/pixmaps"
#endif

#ifndef IMAGE_EXT
#	define IMAGE_EXT	"png"
#endif

/* === G L O B A L S ======================================================= */

/* === P R O C E D U R E S ================================================= */

/* === M E T H O D S ======================================================= */

Dockable::Dockable(const synfig::String& name,const synfig::String& local_name,Gtk::StockID stock_id_):
//	Gtk::Window(Gtk::WINDOW_TOPLEVEL),
	name_(name),
	local_name_(local_name),
//	dialog_settings(this,name),
	title_label_(local_name,Gtk::ALIGN_LEFT),
	stock_id_(stock_id_)
{
	parent_=0;
	scrolled_=0;

	use_scrolled_=true;

	//set_title(local_name);
	//set_type_hint(Gdk::WINDOW_TYPE_HINT_UTILITY);


	title_label_.show();

	attach_dnd_to(title_label_);

	//scrolled_.set_policy(Gtk::POLICY_AUTOMATIC,Gtk::POLICY_AUTOMATIC);
	//scrolled_.show();
	//scrolled_.set_shadow_type(Gtk::SHADOW_NONE);

	toolbar_=0;
	//button_box_.show();

	Gtk::Table* table(this);

	{
		title_label_.set_padding(0,0);
		Gtk::EventBox* event_box(manage(new Gtk::EventBox()));
		event_box->set_border_width(0);
		event_box->add(title_label_);
		//table->attach(*event_box, 0, 1, 0,1, Gtk::EXPAND|Gtk::FILL, Gtk::SHRINK|Gtk::FILL, 0, 0);

		header_box_.pack_start(*event_box);

		attach_dnd_to(*event_box);
		event_box->show();
	//	event_box->set_events(Gdk::ALL_EVENTS_MASK); //!< \todo change this to only allow what is necessary for DnD


		Gtk::Button* bttn_close(manage(new Gtk::Button(_("X"))));
		//table->attach(*bttn_close, 1, 2, 0,1, Gtk::SHRINK|Gtk::FILL, Gtk::SHRINK|Gtk::FILL, 0, 0);
		header_box_.pack_end(*bttn_close,false,false);
		bttn_close->show();
		bttn_close->set_relief(Gtk::RELIEF_NONE);
		bttn_close->signal_clicked().connect(sigc::mem_fun(*this,&Dockable::detach));
		bttn_close->set_border_width(0);
		dynamic_cast<Gtk::Misc*>(bttn_close->get_child())->set_padding(0,0);
	}

	prev_widget_=manage(new Gtk::Label(" "));

	//table->attach(header_box_, 0, 1, 0,1, Gtk::SHRINK|Gtk::FILL, Gtk::SHRINK|Gtk::FILL, 0, 0);
	table->attach(*prev_widget_, 0, 1, 1,2, Gtk::EXPAND|Gtk::FILL, Gtk::EXPAND|Gtk::FILL, 0, 0);
	//table->attach(*toolbar_, 0, 1, 2,3, Gtk::EXPAND|Gtk::FILL, Gtk::SHRINK|Gtk::FILL, 0, 0);
	set_toolbar(*manage(new Gtk::Toolbar));
	table->show();

	prev_widget_->show();

	set_size_request(175,120);
	//scrolled_.set_shadow_type(Gtk::SHADOW_NONE);

}

Dockable::~Dockable()
{
	if(scrolled_)
	{
		delete scrolled_;
		scrolled_=0;
	}

	/*if(App::dock_manager)try{
		App::dock_manager->unregister_dockable(*this);
		std::list<Dockable*>::iterator iter;
		for(iter=App::dock_manager->dockable_list_.begin();iter!=App::dock_manager->dockable_list_.end();++iter)
			if(*iter==this)
			{
				App::dock_manager->dockable_list_.erase(iter);
				return;
			}
	} catch(...) { }
*/
	//if(App::dock_manager)
	//	App::dock_manager->dockable_list_.erase(this);
}

void
Dockable::attach_dnd_to(Gtk::Widget& widget)
{
	std::list<Gtk::TargetEntry> listTargets;
	listTargets.push_back( Gtk::TargetEntry("DOCK") );

	widget.drag_source_set(listTargets);
	widget.drag_source_set_icon(get_stock_id());
	widget.drag_dest_set(listTargets);


	widget.signal_drag_data_get().connect(sigc::mem_fun(*this,&Dockable::on_drag_data_get));
	widget.signal_drag_end().connect(sigc::mem_fun(*this,&Dockable::on_drag_end));
	widget.signal_drag_begin().connect(sigc::mem_fun(*this,&Dockable::on_drag_begin));
	widget.signal_drag_data_received().connect(sigc::mem_fun(*this,&Dockable::on_drag_data_received));
}

void
Dockable::on_drag_data_received(const Glib::RefPtr<Gdk::DragContext>& context, int, int, const Gtk::SelectionData& selection_data, guint, guint time)
{
	if ((selection_data.get_length() >= 0) && (selection_data.get_format() == 8))
	{
		Dockable& dockable(**reinterpret_cast<Dockable**>(const_cast<guint8*>(selection_data.get_data())));

		if(dockable.parent_ != parent_)
			parent_->add(dockable,parent_->page_num(*this));
		else
			parent_->reorder_child(dockable,parent_->page_num(*this));
		dockable.present();
		context->drag_finish(true, false, time);
		return;
	}

	context->drag_finish(false, false, time);
}

void
Dockable::on_drag_end(const Glib::RefPtr<Gdk::DragContext>&/*context*/)
{
	if(!dnd_success_)
	{
		detach();
		present();
	}
}

void
Dockable::on_drag_begin(const Glib::RefPtr<Gdk::DragContext>&/*context*/)
{
	dnd_success_=false;
}

void
Dockable::on_drag_data_get(const Glib::RefPtr<Gdk::DragContext>&, Gtk::SelectionData& selection_data, guint /*info*/, guint /*time*/)
{
	Dockable* tmp(this);
	dnd_success_=true;

	selection_data.set(8, reinterpret_cast<const guchar*>(&tmp), 4);
}

void
Dockable::set_local_name(const synfig::String& local_name)
{
	//set_title(local_name);
	title_label_.set_text(local_name);
}

void
Dockable::clear()
{
	//if(!toolbar_->children().empty())
	//	toolbar_->children().clear();
	set_toolbar(*manage(new Gtk::Toolbar));

}

void
Dockable::set_toolbar(Gtk::Toolbar& toolbar)
{
	if(toolbar_)remove(*toolbar_);
	toolbar_=0;
	toolbar_=&toolbar;
	if(toolbar_)
	{
		attach(*toolbar_, 0, 1, 2,3, Gtk::EXPAND|Gtk::FILL, Gtk::SHRINK|Gtk::FILL, 0, 0);
		gtk_toolbar_set_icon_size(toolbar_->gobj(),GtkIconSize(1)/*GTK_ICON_SIZE_MENU*/);
		toolbar_->show();
	}
}

bool
Dockable::clear_previous()
{
	prev_widget_=0;
	prev_widget_delete_connection.disconnect();
	return false;
}

void
Dockable::add(Gtk::Widget& x)
{
	if(prev_widget_)
	{
		remove(*prev_widget_);
		//prev_widget_=0;
		clear_previous();
	}

	if(scrolled_)
	{
		delete scrolled_;
		scrolled_=0;
	}

	if(use_scrolled_)
	{
		scrolled_=new Gtk::ScrolledWindow;

		scrolled_->add(x);

		attach(*scrolled_, 0, 1, 1,2, Gtk::EXPAND|Gtk::FILL, Gtk::EXPAND|Gtk::FILL, 0, 0);

		x.show();

		scrolled_->show();

		scrolled_->set_shadow_type(Gtk::SHADOW_NONE);
		scrolled_->set_policy(Gtk::POLICY_AUTOMATIC,Gtk::POLICY_AUTOMATIC);
		prev_widget_=scrolled_;
	}
	else
	{
		attach(x, 0, 1, 1,2, Gtk::EXPAND|Gtk::FILL, Gtk::EXPAND|Gtk::FILL, 0, 0);
		x.show();
		prev_widget_=&x;
	}
	prev_widget_delete_connection=prev_widget_->signal_delete_event().connect(
		sigc::hide(
			sigc::mem_fun(
				*this,
				&Dockable::clear_previous
			)
		)
	);
}

Gtk::ToolButton*
Dockable::add_button(const Gtk::StockID& stock_id, const synfig::String& tooltip)
{
	if(!toolbar_)
		set_toolbar(*manage(new Gtk::Toolbar));

	//Gtk::IconSize iconsize(4);
	//Gtk::IconSize iconsize(Gtk::IconSize::from_name("synfig-small_icon"));

	Gtk::ToolButton* ret(manage(new Gtk::ToolButton(stock_id)));
	//Gtk::Image* icon(manage(new Gtk::Image(stock_id,iconsize)));
	//ret->add(*icon);
	//ret->set_relief(Gtk::RELIEF_HALF);
	//ret->set_relief(Gtk::RELIEF_NONE);
	ret->set_label(tooltip);
	if (toolbar_->get_tooltips_object())
		toolbar_->get_tooltips_object()->set_tip(*ret,tooltip);

	ret->show();
	//icon->show();
	toolbar_->set_tooltips(true);

	toolbar_->append(*ret);
	//button_box_.pack_start(*ret,false,false);
	//get_action_area()->pack_start(*ret,false,false);
	//add_action_widget(*ret,1);
	return ret;
}


void
Dockable::detach()
{
	if(parent_)
		parent_->remove(*this);
}

void
Dockable::present()
{
	if(parent_)
	{
		parent_->set_current_page(parent_->page_num(*this));
		parent_->present();
	}
	else
	{
		DockDialog* dock_dialog(new DockDialog());
		dock_dialog->get_dock_book().add(*this);
		//if(get_name()=="canvases")
		//	dock_dialog->set_composition_selector(true);
		dock_dialog->present();
	}
}

Gtk::Widget*
Dockable::create_tab_label()
{
	Gtk::EventBox* event_box(manage(new Gtk::EventBox()));

	attach_dnd_to(*event_box);

	{
		Gtk::StockID stock_id(get_stock_id());
		Gtk::StockItem item;

		// Check to make sure the icon is valid
		if(Gtk::Stock::lookup(stock_id,item))
		{
			Gtk::Image* icon(manage(new Gtk::Image(stock_id,Gtk::IconSize(4))));
			event_box->add(*icon);
			tooltips_.set_tip(*event_box,get_local_name());
			icon->show();
		}
		else
		{
			// Bad icon, try to make a label

			Glib::ustring text(get_local_name());

			Gtk::Label* label(manage(new Gtk::Label(text)));
			event_box->add(*label);
			label->show();
		}
	}

	return event_box;
}