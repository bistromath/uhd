//
// Copyright 2014 Ettus Research LLC
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <uhd/rfnoc/predistorter_block_ctrl.hpp>
#include <uhd/convert.hpp>
#include <uhd/utils/msg.hpp>

using namespace uhd::rfnoc;

class predistorter_block_ctrl_impl : public predistorter_block_ctrl
{
public:
    const size_t n_taps = 4096;

    static boost::uint32_t AXIS_CONFIG_TAPS(size_t which) {
        return AXIS_CONFIG_BUS+2*which;
    }

    static boost::uint32_t AXIS_CONFIG_TAPS_TLAST(size_t which) {
        return AXIS_CONFIG_BUS+2*which+1;
    }

    UHD_RFNOC_BLOCK_CONSTRUCTOR(predistorter_block_ctrl),
        _item_type("sc16") // We only support sc16 in this block
    {
        UHD_MSG(status) << "predistorter_block::predistorter_block() init..." << std::endl;

        // Default to one-to-one output
        // TODO FIXME: what's your scaling?
        std::vector<int> default_taps(n_taps);
        for(int i=0; i<n_taps; i++) {
            default_taps[i] = i*(32768/n_taps);
        }

        for(int i=0; i<4; i++) {
            UHD_MSG(status) << "predistorter_block: writing taps for input " << i << "..." << std::endl;
            set_taps(i, default_taps);
        }
    }

    void set_taps(size_t which, const std::vector<int> &taps)
    {
        UHD_RFNOC_BLOCK_TRACE() << "predistorter_block::set_taps()" << std::endl;
        if (taps.size() != n_taps) {
            throw uhd::value_error(str(
                boost::format("predistorter block: invalid number of taps (must be %d)!\n")
                % n_taps
            ));
        }

        // Write taps via the reload bus
        // Note that we're sending taps.size() + 1 taps in total.
        // This is done because there are actually ntaps*2 taps
        // in the predistorter. The latter half are negative.
        // We send this last t
        for (size_t i = 0; i < taps.size(); i++) {
            sr_write(AXIS_CONFIG_TAPS(which), boost::uint32_t(taps[i]));
        }
        for (size_t i = 0; i < taps.size()-1; i++) {
            sr_write(AXIS_CONFIG_TAPS(which), boost::uint32_t(0));
        }
        // Assert tlast when sending the spinal tap (haha, it's actually the final tap).
        sr_write(AXIS_CONFIG_TAPS_TLAST(which), boost::uint32_t(0));
    }

private:
    const std::string _item_type;
};

UHD_RFNOC_BLOCK_REGISTER(predistorter_block_ctrl, "Predistorter");
