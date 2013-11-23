#ifndef _X_CLIENT_THUMBNAIL_GL_HPP
#define _X_CLIENT_THUMBNAIL_GL_HPP

#include <fstream>
#include <unordered_map>
#include <xcb/xcb.h>
#include <xcb/damage.h>

#include "x_xrm.hpp"
#include "observer.hpp"
#include "thumbnail_t.hpp"
#include "x_event_handler_t.hpp"
#include "x_client.hpp"
#include "x_client_icon.hpp"
#include "x_client_name.hpp"

// http://svn.enlightenment.org/svn/e/tags/evas-1.0.2/src/modules/engines/xrender_x11/evas_engine_xcb_render.c
#define DOUBLE_TO_FIXED(d) ((xcb_render_fixed_t) ((d) * 65536))

class x_connection;

class x_client_thumbnail : public x_event_handler_t
                         , public thumbnail_t
                         , public observer<x::xrm>
                         , public observer<x_client>
                         , public observer<x_client_name>
{
  public:
    friend bool operator==(const x_client_thumbnail &, const xcb_window_t &);
    friend bool operator==(const xcb_window_t &, const x_client_thumbnail &);

    x_client_thumbnail(x_connection & c,
                       x::xrm & xrm,
                       const rectangle & rect,
                       const xcb_window_t & window = XCB_NONE);

    ~x_client_thumbnail(void);

    bool operator==(const x_client_thumbnail & other)
    {
      return _thumbnail_window == other._thumbnail_window;
    }

    thumbnail_t & show(void);
    thumbnail_t & hide(void);
    thumbnail_t & select(void);
    thumbnail_t & update(void);
    thumbnail_t & update(const rectangle &);
    const rectangle & rect(void);
    const xcb_window_t & id(void);
    const xcb_window_t & window(void);
    thumbnail_t & highlight(bool want_highlight);

    bool handle(xcb_generic_event_t * ge);
    void notify(x::xrm *);
    void notify(x_client *);
    void notify(x_client_name *);

    class factory : public thumbnail_t::factory {
      public:
        factory(x_connection & c, x::xrm & xrm);
        thumbnail_t::ptr
          make(const xcb_window_t &, const rectangle &) const;

      private:
        x_connection & _c;
        x::xrm & _xrm;
    };

  private:
    x_connection & _c;
    x::xrm & _xrm;
    x_client _x_client;
    x_client_icon _x_client_icon;
    x_client_name _x_client_name;

    double _scale;
    double _icon_scale_x;
    double _icon_scale_y;
    double _title_scale_x;
    double _title_scale_y;

    bool _purge = false;
    bool _visible = false;
    bool _highlight = false;
    bool _configure_thumbnail = true;
    bool _configure_highlight = true;

    rectangle _rectangle;

    xcb_window_t _thumbnail_window;
    xcb_damage_damage_t _damage;

    const int _gl_pixmap_config[3] = { GLX_BIND_TO_TEXTURE_RGBA_EXT, True, None };

    int _gl_xfb_nconfigs = 0;
    GLXFBConfig * _gl_xfb_configs = NULL;
    GLuint _gl_texture_id[3];
    GLXContext _gl_ctx = XCB_NONE;
    GLXPixmap _gl_pixmap[3] = { XCB_NONE, XCB_NONE, XCB_NONE };
    std::unordered_map<std::string, GLuint> _programs;

    // >> config options

    unsigned int _icon_size;
    unsigned int _border_width;

    // red, green, blue, alpha
    std::tuple<double, double, double, double> _focused_border_color;
    std::tuple<double, double, double, double> _unfocused_border_color;

    // << config options

    void purge(void);
    void update(int x, int y, unsigned int width, unsigned int height);
    void configure_highlight(bool now = false);
    void configure_thumbnail_window(bool now = false);
    void load_texture(GLuint id, const xcb_pixmap_t & p, bool rgba = true);
    void configure_gl(XVisualInfo * vi = NULL);
    void init_gl_shader(void);
    void load_gl_shader(const std::string & filename, const std::string & name);
    void release_gl(void);
    void load_config(void);
    void with_context(std::function<void(void)> f);
    void with_texture(GLuint tid, std::function<void(GLuint &)> f);
};

bool
operator==(const x_client_thumbnail & thumbnail, const xcb_window_t & window);

bool
operator==(const xcb_window_t & window, const x_client_thumbnail & thumbnail);

#endif
