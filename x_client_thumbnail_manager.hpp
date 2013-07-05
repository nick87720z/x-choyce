#ifndef _X_CLIENT_THUMBNAIL_MANAGER_HPP
#define _X_CLIENT_THUMBNAIL_MANAGER_HPP

#include <vector>
#include <iterator>

#include "cyclic_iterator.hpp"
#include "chooser_t.hpp"
#include "thumbnail_t.hpp"
#include "x_event_handler_t.hpp"
#include "x_connection.hpp"

class x_client_thumbnail_manager : public chooser_t {
  public:
    x_client_thumbnail_manager(
        x_connection & c,
        thumbnail_factory_t<std::vector> * thumbnail_factory);

    ~x_client_thumbnail_manager(void) {}

    void show(void);
    void hide(void);
    void next(void);
    void prev(void);
    void select(void);

  private:
    typedef std::vector<thumbnail_t::thumbnail_ptr> thumbnail_list_t;
    typedef std::back_insert_iterator<thumbnail_list_t> thumbnail_inserter_t;
    typedef const_cyclic_iterator<thumbnail_list_t> thumbnail_cyclic_iterator;

    x_connection & _c;
    thumbnail_factory_t<std::vector> * _thumbnail_factory;

    const unsigned int id = 0;

    std::vector<thumbnail_t::thumbnail_ptr> _thumbnails;
    thumbnail_cyclic_iterator _thumbnail_cyclic_iterator;

    void next_or_prev(bool next);
};

#endif
