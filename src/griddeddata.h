/* Copyright (c) 2018 Big Ladder Software LLC. All rights reserved.
* See the LICENSE file for additional terms and conditions. */

#ifndef GRIDDEDDATA_H_
#define GRIDDEDDATA_H_

#include <vector>
#include <cfloat>

#include "Eigen/Dense"


namespace Btwxt {

    enum class Method {
        CONSTANT, LINEAR, CUBIC
    };

    class GridAxis {
        // A single input dimension of the performance space
    public:
        GridAxis();

        explicit GridAxis(std::vector<double> grid_vector,
                 Method extrapolation_method = Method::CONSTANT,
                 Method interpolation_method = Method::LINEAR,
                 std::pair<double, double> extrapolation_limits = {-DBL_MAX, DBL_MAX}
        );

        const std::vector<double> grid;
        std::vector<std::vector<double> > spacing_multiplier;
        Method extrapolation_method;
        Method interpolation_method;
        std::pair<double, double> extrapolation_limits;

        // std::pair<double> extrapolation_bounds;  <-- to add later
        // bool is_regular;  <-- to add later

        std::size_t get_length();

        void set_interp_method(Method interpolation_method);
        void set_extrap_method(Method extrapolation_method);
        void set_extrap_limits(std::pair<double, double> extrap_limits);

        double get_spacing_multiplier(const std::size_t &flavor, const std::size_t &index);

    private:
        std::vector<std::vector<double> > calc_spacing_multipliers();
        void check_grid_sorted();
        void check_extrap_limits();
    };


    class GriddedData {
    public:
        GriddedData();

        GriddedData(
                std::vector<std::vector<double> > grid,
                std::vector<std::vector<double> > values
        );

        GriddedData(
                std::vector<GridAxis> grid_axes,
                std::vector<std::vector<double> > values
                );

        explicit GriddedData(
                std::vector<GridAxis> grid_axes
        );

        std::size_t get_ndims();

        std::size_t get_num_tables();

        std::size_t add_value_table(std::vector<double>& value_vector);

        std::vector<double> get_grid_vector(const std::size_t &dim);

        std::pair<double, double> get_extrap_limits(const std::size_t &dim);

        std::vector<double> get_values(const std::vector<std::size_t> &coords);

        template <typename T>
        Eigen::ArrayXd get_column(const std::vector<T> &coords);

        Eigen::ArrayXd get_column_near(
                std::vector<std::size_t> coords, const std::size_t &dim, const int &i);

        Eigen::ArrayXd get_column_near(
                std::vector<std::size_t> coords, const std::vector<int> &translation);

        Eigen::ArrayXd get_column_near_safe(
                const std::vector<std::size_t>& coords, std::vector<int> translation);

        double get_axis_spacing_mult(const std::size_t &dim,
                                     const std::size_t &flavor, const std::size_t &index);

        std::vector<Method> get_interp_methods();

        std::vector<Method> get_extrap_methods();
        // double get_value(std::size_t table_index, std::vector<std::size_t> coords);

        void set_axis_extrap_method(const std::size_t &dim, Method);

        void set_axis_extrap_limits(const std::size_t &dim,
                                    const std::pair<double, double> &extrap_limits);

        void set_axis_interp_method(const std::size_t &dim, Method);

        Eigen::ArrayXXd value_tables;
        std::size_t num_values;
        std::vector<std::size_t> dimension_lengths;

    private:
        void construct_axes(const std::vector<std::vector<double> > &grid);

        Eigen::ArrayXXd construct_values(std::vector<double> &value_vector);
        Eigen::ArrayXXd construct_values(
                const std::vector<std::vector<double> > &values
        );
        Eigen::Map<Eigen::ArrayXd> fill_value_row(std::vector<double> &value_vector,
                const std::size_t& num_values);
        std::vector<GridAxis> grid_axes;
        std::size_t ndims;
        std::size_t num_tables;
    };


// free functions
    bool free_check_sorted(std::vector<double>);

    template <typename T>
    std::size_t locate_coords(
            const std::vector<T> &coords,
            const std::vector<std::size_t> &dimension_lengths
    );

    std::vector<double> eigen_to_vector(Eigen::ArrayXd &);


}
#endif // GRIDDEDDATA_H_
