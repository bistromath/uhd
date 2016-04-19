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

#ifndef INCLUDED_LIBUHD_RFNOC_fir_block_ctrl_HPP
#define INCLUDED_LIBUHD_RFNOC_fir_block_ctrl_HPP

#include <uhd/rfnoc/source_block_ctrl_base.hpp>
#include <uhd/rfnoc/sink_block_ctrl_base.hpp>

namespace uhd {
    namespace rfnoc {

/*! \brief Block controller for the GMRR predistorter block
 *
 * The predistorter has the following features:
 * - One input and four output ports
 * - Configurable taps, but fixed number of taps (128 each, x4)
 * - Supports data type sc16 (16-Bit fix-point complex samples)
 *   The "Q" channel is ignored.
 *
 */
class UHD_API predistorter_block_ctrl : public source_block_ctrl_base, public sink_block_ctrl_base
{
public:
    UHD_RFNOC_BLOCK_OBJECT(predistorter_block_ctrl)

    //! Configure the filter taps.
    //
    // The length of \p taps must correspond the number of taps
    // in this block. If it's shorter, zeros will be padded.
    // If it's longer, throws a uhd::value_error.
    virtual void set_taps(size_t which, const std::vector<int> &taps) = 0;

}; /* class predistorter_block_ctrl*/

}} /* namespace uhd::rfnoc */

#endif /* INCLUDED_LIBUHD_RFNOC_predistorter_block_ctrl_HPP */
// vim: sw=4 et:
