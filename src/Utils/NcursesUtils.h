/*
 * NcursesUtils.h
 *
 * Created on: Jul 30, 2015
 *     Author: dpayne
 */

#ifndef _VIS_NCURSES_UTILS_H
#define _VIS_NCURSES_UTILS_H

#include "Domain/VisConstants.h"
#include "Domain/VisTypes.h"
#include "Utils/Logger.h"
#include "Utils/Utils.h"

#include <map>

#ifdef NCURSESW
#include <ncursesw/ncurses.h>
#else
#include <ncurses.h>
#endif

namespace vis
{

const static std::map<std::string, int16_t> g_vis_color_map{
    {"black", COLOR_BLACK},     {"blue", COLOR_BLUE},     {"cyan", COLOR_CYAN},
    {"green", COLOR_GREEN},     {"yellow", COLOR_YELLOW}, {"red", COLOR_RED},
    {"magenta", COLOR_MAGENTA}, {"white", COLOR_WHITE},
};

class NcursesUtils
{

  public:
    /**
     * Convert a string to a color index. If the string is numeric the string is
     * convert to a number and returned as a ColorIndex. A color can be called
     * by name if it is one of the 8 basic colors:
     * black,red,green,yellow,blue,magenta,cyan,white.
     *
     * If the string is empty or if it is a name of a color that is not one of
     * the 8 basic colors, then black is returned.
     */
    static inline vis::ColorIndex to_basic_color(const std::string &str)
    {
        if (str.empty())
        {
            return COLOR_BLACK;
        }

        auto iter = g_vis_color_map.find(vis::Utils::lowercase(str));
        if (iter != g_vis_color_map.end())
        {
            return iter->second;
        }

        if (!vis::Utils::is_numeric(str))
        {
            return -1;
        }

        return static_cast<vis::ColorIndex>(std::atoi(str.c_str()));
    }

    /**
     * Returns number of colors supported in the current terminal
     */
    static inline int32_t number_of_colors_supported()
    {
        return COLORS;
    }

    /**
     * Approximate a RGB color to a ncurses color pair. This idea was originally
     * taken from ruby's rainbow gem https://github.com/sickill/rainbow
     *
     * This is not a true RGB color, just an approximation. This is also
     * dependent on terminal settings. For example, if the terminal colors have
     * all been set to red, then they will still all be red.
     */
    static inline int16_t to_ansi_color(const int16_t red, const int16_t green,
                                        const int16_t blue)
    {
        return 16 + static_cast<int16_t>((to_ansi_color_domain(red) * 36.0) +
                                         (to_ansi_color_domain(green) * 6.0) +
                                         (to_ansi_color_domain(blue) * 1.0));
    }

    static inline int32_t get_window_height()
    {
        int32_t width, height;

        // getmaxyx is actually a macro, not a function, so pass it two integers
        getmaxyx(stdscr, height, width);

        return height;
    }

    static inline int32_t get_window_width()
    {
        int32_t width, height;

        // getmaxyx is actually a macro, not a function, so pass it two integers
        getmaxyx(stdscr, height, width);

        return width;
    }

    static inline int32_t get_user_input()
    {
        timeout(0);
        return getch();
    }

  private:
    explicit NcursesUtils();

    virtual ~NcursesUtils();

    static inline int16_t to_ansi_color_domain(const int16_t color)
    {
        return static_cast<int16_t>(6.0 * static_cast<double>(color) / 256.0);
    }
};
}

#endif
