#include "pch.hpp"
#include "describe_config.hpp"
#include "ids.hpp"

namespace be::cli {
namespace {

///////////////////////////////////////////////////////////////////////////////
ct::TableConfig default_table_config() {
   using namespace ct;
   TableConfig config;

   {
      config.headers.emplace_back();
      RowConfig& row = config.headers.back();

      row.box.foreground = LogColor::white;

      row.cells.emplace_back();
      CellConfig& cell = row.cells.back();

      cell.box.sides[BoxConfig::right_side].margin = 1;
   }

   {
      config.rows.emplace_back();
      RowConfig& row = config.rows.back();

      row.box.sides[BoxConfig::bottom_side].margin = 1;

      row.cells.emplace_back();
      CellConfig& cell = row.cells.back();

      cell.box.sides[BoxConfig::left_side].padding = 3;
      cell.box.sides[BoxConfig::right_side].margin = 1;
   }

   config.box.sides[BoxConfig::bottom_side].margin = 1;

   return config;
}

///////////////////////////////////////////////////////////////////////////////
ct::TableConfig compact_options_table_config() {
   using namespace ct;
   TableConfig config;

   {
      config.rows.emplace_back();
      RowConfig& row = config.rows.back();

      row.box.sides[BoxConfig::left_side].margin = 3;

      { // option names
         row.cells.emplace_back();
         CellConfig& cell = row.cells.back();

         cell.box.foreground = LogColor::yellow;
         cell.box.sides[BoxConfig::right_side].margin = 2;
         cell.min_width = 24;
      }

      { // option description
         row.cells.emplace_back();
         CellConfig& cell = row.cells.back();

         cell.box.sides[BoxConfig::right_side].margin = 1;
         cell.min_width = 24;
      }

      row.cell_repeat_modulo = 1;
   }

   config.box.sides[BoxConfig::bottom_side].margin = 1;

   return config;
}

///////////////////////////////////////////////////////////////////////////////
ct::TableConfig man_options_table_config() {
   using namespace ct;
   TableConfig config;

   { // option names
      config.rows.emplace_back();
      RowConfig& row = config.rows.back();

      row.box.foreground = LogColor::yellow;
      row.box.sides[BoxConfig::left_side].margin = 3;
      row.box.sides[BoxConfig::right_side].margin = 1;
   }

   { // option description
      config.rows.emplace_back();
      RowConfig& row = config.rows.back();

      row.box.sides[BoxConfig::bottom_side].margin = 1;
      row.box.sides[BoxConfig::left_side].margin = 3;
      row.box.sides[BoxConfig::right_side].margin = 1;

      row.cells.emplace_back();
      CellConfig& cell = row.cells.back();

      cell.box.sides[BoxConfig::left_side].padding = 5;
   }

   config.box.sides[BoxConfig::bottom_side].margin = 1;
   config.row_repeat_modulo = 2;

   return config;
}

///////////////////////////////////////////////////////////////////////////////
ct::TableConfig exit_code_table_config() {
   using namespace ct;
   TableConfig config;

   {
      config.rows.emplace_back();
      RowConfig& row = config.rows.back();

      row.box.sides[BoxConfig::left_side].margin = 3;

      { // exit code
         row.cells.emplace_back();
         CellConfig& cell = row.cells.back();

         cell.box.foreground = LogColor::blue;
         cell.box.sides[BoxConfig::right_side].margin = 2;
         cell.min_width = 3;
         cell.box.align = BoxConfig::align_right;
      }

      { // description
         row.cells.emplace_back();
         CellConfig& cell = row.cells.back();

         cell.box.sides[BoxConfig::right_side].margin = 1;
      }

      row.cell_repeat_modulo = 1;
   }

   config.box.sides[BoxConfig::bottom_side].margin = 1;

   return config;
}

///////////////////////////////////////////////////////////////////////////////
ct::TableConfig synopsis_table_config() {
   using namespace ct;
   TableConfig config;

   {
      config.rows.emplace_back();
      RowConfig& row = config.rows.back();

      row.box.foreground = LogColor::blue;
      row.box.sides[BoxConfig::left_side].margin = 3;
      row.box.sides[BoxConfig::right_side].margin = 1;
   }

   config.box.sides[BoxConfig::bottom_side].margin = 1;

   return config;
}

///////////////////////////////////////////////////////////////////////////////
ct::TableConfig examples_table_config() {
   using namespace ct;
   TableConfig config;

   { // command
      config.rows.emplace_back();
      RowConfig& row = config.rows.back();

      row.box.foreground = LogColor::blue;
      row.box.sides[BoxConfig::left_side].margin = 3;
      row.box.sides[BoxConfig::right_side].margin = 1;
   }

   { // description
      config.rows.emplace_back();
      RowConfig& row = config.rows.back();

      row.box.sides[BoxConfig::bottom_side].margin = 1;
      row.box.sides[BoxConfig::left_side].margin = 3;
      row.box.sides[BoxConfig::right_side].margin = 1;

      row.cells.emplace_back();
      CellConfig& cell = row.cells.back();

      cell.box.sides[BoxConfig::left_side].padding = 5;
   }

   config.box.sides[BoxConfig::bottom_side].margin = 1;
   config.row_repeat_modulo = 2;

   return config;
}

} // be::cli::()

///////////////////////////////////////////////////////////////////////////////
DescribeConfig default_describe_config(bool man_style_options) {
   DescribeConfig config;

   config.push_back({ ids::cli_describe_section_prologue, S(), default_table_config() });
   config.push_back({ ids::cli_describe_section_intro, S(), default_table_config() });
   config.push_back({ ids::cli_describe_section_synopsis, "SYNOPSIS", synopsis_table_config() });
   config.push_back({ ids::cli_describe_section_abstract, "DESCRIPTION", default_table_config() });

   if (man_style_options) {
      config.push_back({ ids::cli_describe_section_options_manstyle, "OPTIONS", man_options_table_config() });
   } else {
      config.push_back({ ids::cli_describe_section_options_compact, "OPTIONS", compact_options_table_config() });
   }
   
   config.push_back({ ids::cli_describe_section_summary, S(), default_table_config() });
   config.push_back({ ids::cli_describe_section_exitcodes, "EXIT STATUS CODES", exit_code_table_config() });
   config.push_back({ ids::cli_describe_section_examples, "EXAMPLES", examples_table_config() });
   config.push_back({ ids::cli_describe_section_conclusion, S(), default_table_config() });
   config.push_back({ ids::cli_describe_section_license, "LICENSE", default_table_config() });
   config.push_back({ ids::cli_describe_section_epilogue, S(), default_table_config() });

   return config;
}

} // be::cli
