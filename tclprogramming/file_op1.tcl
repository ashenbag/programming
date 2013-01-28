#!/bin/tclsh

set days_of_week [list "Sunday" "Monday" "Tuesday" "Wednesday" "Thursday" "Friday" "Saturday"]
proc day_to_number {day} {
	switch $day {
		"Sunday" {
			return 1
		}
		"Monday" {
			return 2
		}
		"Tuesday" {
			return 3
		}
		"Wednesday" {
			return 4
		}
		"Thursday" {
			return 5
		}
		"Friday" {
			return 6
		}
		"Saturday" {
			return 7
		}
	}
}

proc number_to_day {number} {
	global days_of_week
	return [lindex $days_of_week $number]
}

proc file_data_to_array {} {
	global data_array
	global row_index_list

	set data_fp [open "data_file1" r]
	set line_count 0
	set key ""
	while { [gets $data_fp data_line] >=0} {
		incr line_count
		if {$line_count > 1} {
			set item_count 0
			foreach list_item $data_line {
				if {$item_count == 0} {
					set data_row_index $list_item
					lappend row_index_list $data_row_index				
				} else {
					array set data_array [list [append key $item_count "." $data_row_index] $list_item]
					set key ""
				}
				incr item_count
			}
		}
	}
}

proc get_per_row_count {row} {
	global data_array
	set total 0
	foreach element [array names data_array "*\.$row"] {
		set total [expr $total + $data_array($element)]
	}
	return $total
}
proc get_per_col_count {col_name} {
	global data_array
	set total 0
	set col [day_to_number $col_name]
	foreach element [array names data_array "$col\.*"] {
		set total [expr $total + $data_array($element)]
	}
	return $total
}

file_data_to_array
foreach col $days_of_week {
	puts [format "%-15s %d" "$col:" [get_per_col_count $col]]
}
puts "******"
foreach row $row_index_list {
	puts [format "%-15s %d" "$row:" [get_per_row_count $row]]
}
