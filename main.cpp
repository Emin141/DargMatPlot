#define EPSILON 1e-10

#include <iostream>
#include <fstream>
#include <map>
#include <cmath>
#include <vector>

using ZMap = std::map<std::pair<double, double>, double>;

struct Dimensions
{
    int x, y;
} dimensions;

void parse_csv(const std::string &&filename, ZMap &zMap)
{
    std::ifstream csv_file{filename, std::ios::in};
    if (!csv_file.is_open())
    {
        throw std::runtime_error("Could not read CSV file " + filename);
    }
    else
    {
        std::string value{};
        std::getline(csv_file, value);

        // IMPORTANT
        // Need to get the first X value in order to check for the number of y values
        double first_x_value{};
        int number_of_unique_y_values{1};
        bool number_of_unique_y_values_found{false};

        {
            double x, y, z;

            std::getline(csv_file, value, ',');
            x = std::atof(value.c_str());
            first_x_value = x;

            std::getline(csv_file, value, ',');
            y = std::atof(value.c_str());

            std::getline(csv_file, value, '\n');
            z = std::atof(value.c_str());

            zMap[std::make_pair(x, y)] = z;

            // Now all other rows are calculated
            while (!csv_file.eof())
            {
                std::getline(csv_file, value, ',');
                x = std::atof(value.c_str());
                if (!number_of_unique_y_values_found)
                {
                    if (fabs(x - first_x_value) < EPSILON)
                    {
                        number_of_unique_y_values++;
                    }
                    else if (fabs(x - first_x_value) > EPSILON)
                    {
                        number_of_unique_y_values_found = true;
                    }
                }

                std::getline(csv_file, value, ',');
                y = std::atof(value.c_str());

                std::getline(csv_file, value, '\n');
                z = std::atof(value.c_str());

                zMap[std::make_pair(x, y)] = z;
            }
        }
        dimensions.x = zMap.size() / number_of_unique_y_values;
        dimensions.y = number_of_unique_y_values;

        csv_file.close();
    }
}

void fix_dimensions_error(ZMap &zMap) noexcept
{
    int i{0};
    for (auto it = zMap.begin(); it != zMap.end(); ++it)
    {
        if (i++ == (dimensions.x + dimensions.y) / 2)
        {
            zMap.erase(it);
            return;
        }
    }
}

int main(int argc, char **argv)
{
    try
    {
        if (argc < 2)
        {
            throw std::runtime_error("Fatal error: please provide a filename to parse.");
        }

        std::vector<double> x, y, z;
        {
            ZMap zMap{};
            parse_csv(argv[1], zMap);

            // x parsing loop
            for (auto it = zMap.begin(); it != zMap.end(); std::advance(it, dimensions.y))
            {
                x.push_back(it->first.first);
            }
            // y parsing loop
            double index{0};
            for (auto it = zMap.begin(); index < dimensions.y; ++it, index++)
            {
                y.push_back(it->first.second);
            }
            // z parsing loop
            // for(auto _x:x){
            //     for(auto _y:y){
            //         z.push_back(zMap.at(std::make_pair(_x, _y)));
            //     }
            // }

            for(auto val:zMap){
                z.push_back(val.second);
            }
            if (dimensions.x != dimensions.y)
            {
                fix_dimensions_error(zMap);
            }
        }

        std::ofstream output{"output.dat"};
        if (!output.is_open())
        {
            throw std::runtime_error("Error: failed to create output file");
        }
        for (auto value : x)
        {     
            output << value << " ";
        }
        output << std::endl;
        for (auto value : y)
        {
            output << value << " ";
        }
        output << std::endl;
        for (auto value : z)
        {
            output << value << " ";
        }
        output << std::endl;
        output.close();

        system("python3 plot.py");
        // system("rm output.dat");
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}