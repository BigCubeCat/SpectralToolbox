//
// Created by anton on 1/30/25.
//

#ifndef DECOMPOSER_IMPL_HPP
#define DECOMPOSER_IMPL_HPP
#include "../include/decomposer.h"
#include "../include/extremum_searcher.h"
#include "../include/spline.h"
using namespace emd;

template <typename E>
E get_oscillation_amplitude(const Eigen::Vector<E, Eigen::Dynamic> &trace)
{
    E min = trace.minCoeff();
    E max = trace.maxCoeff();
    return (max - min);
}

template <typename E>
Eigen::Vector<E, Eigen::Dynamic> generate_grid(long count, E step) {
    Eigen::Vector<E, Eigen::Dynamic> result { count };
    for (long i = 0; i < count; i++) {
        result[i] = step * i;
    }
    return result;
}

template <typename E>
Eigen::Vector<E, Eigen::Dynamic> generate_grid(long int count) {
    Eigen::Vector<E, Eigen::Dynamic> result { count };
    for (long i = 0; i < count; i++) {
        result[i] = i;
    }
    return result;
}

template <typename E>
void product_window( Eigen::Vector<E, Eigen::Dynamic> &trace) {
    for (long i =0; i < trace.size(); i++) {
        trace[i] *= 0.65 - 0.35 * std::cos(2 * M_PI * i / trace.size());
    }
}

template <typename E>
std::vector<Eigen::Vector<E, Eigen::Dynamic>>
decomposer<E>::decompose_signal(const Eigen::Vector<E, Eigen::Dynamic> &trace) {
    extremum_searcher<E> searcher;
    Eigen::Vector<E, Eigen::Dynamic> rest(trace);    // остаток от трассы
    std::vector<Eigen::Vector<E, Eigen::Dynamic>> modes;    // результат
    Eigen::Vector<E, Eigen::Dynamic> grid = generate_grid<E>(trace.size()); // сетка для функций
    int cnt = 0;    // переменная основного цикла для ограничения на количество
                    // мод в результате

    // основной цикл
    do {

        Eigen::Vector<E, Eigen::Dynamic> iteration(rest);    // основа для
                                                             // выделяемой моды
        //умножаем трассу на окно Ханна для снятия граничных эффектов
        product_window<E>(iteration);
        for (long i = 0; i < m_max_iterations; i++) {
            //поиск экстремумов
            std::vector<int> top_points = searcher.search_maxima(iteration,false, 8);
            std::vector<int> bottom_points =
                searcher.search_minima(iteration,false, 8);

            //добавляем краевые точки если не хватает (т.е. меньше 3-х точек)
            if (top_points.size() < 3)
            {
                long last_size = top_points.size();
                top_points.resize( last_size + 2);
                for (long i = last_size - 1 ; i >=0; --i)
                {
                    top_points[i + 1] = top_points[i];
                }
                top_points[0] = 0;
                top_points[top_points.size() - 1] = trace.size() - 1;
            }

            if (bottom_points.size() < 3)
            {
                long last_size = bottom_points.size();
                bottom_points.resize( last_size + 2);
                for (long i = last_size - 1 ; i >=0; --i)
                {
                    bottom_points[i + 1] = bottom_points[i];
                }
                bottom_points[0] = 0;
                bottom_points[bottom_points.size() - 1] = trace.size() - 1;
            }

            Eigen::Vector<E, Eigen::Dynamic> top_xs { top_points.size() };
            Eigen::Vector<E, Eigen::Dynamic> bottom_xs { bottom_points.size()};
            Eigen::Vector<E, Eigen::Dynamic> top_ys { top_points.size() };
            Eigen::Vector<E, Eigen::Dynamic> bottom_ys { bottom_points.size() };

            for (long j = 0, end = top_points.size(); j != end; ++j) {
                top_xs[j] = grid[top_points[j]];
                top_ys[j] = iteration[top_points[j]];
            }

            for (long j = 0, end = bottom_points.size(); j != end; ++j) {
                bottom_xs[j] = grid[bottom_points[j]];
                bottom_ys[j] = iteration[bottom_points[j]];
            }
            //std::cout <<top_xs.size() << " " << top_ys.size() << "\n";



            // просчитываем огибающие линии как кубичиские сплайны
            spline top_spline { top_xs, top_ys };
            spline bottom_spline { bottom_xs, bottom_ys };

            Eigen::Vector<E, Eigen::Dynamic> top_envelope =
                top_spline.compute(grid);
            Eigen::Vector<E, Eigen::Dynamic> bottom_envelope =
                bottom_spline.compute(grid);

            //считаем среднее от огибающих - это будет тренд
            Eigen::Vector<E, Eigen::Dynamic> average { grid.size() };
            for (long j = 0, end = grid.size(); j != end; ++j) {
                average[j] = (top_envelope[j] + bottom_envelope[j]) / 2;
            }
            //отнимаем тренд от сигнала и переходим к следующей итерации
            iteration = iteration - average;
        }
        //полученную моду отнимаем от остатка и ищем следующую моду
        modes.push_back(iteration);
        rest = rest - iteration;
        cnt++;

    } while (get_oscillation_amplitude(rest) > m_min_oscillation && cnt < m_max_result_size);

    return modes;
}

#endif    // DECOMPOSER_IMPL_HPP
