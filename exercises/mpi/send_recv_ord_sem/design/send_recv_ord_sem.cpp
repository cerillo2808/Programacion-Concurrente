procedure main:
  declare const previous_process = \
    (process_count + process_number - 1) % process_count
  declare const next_process = (process_number + 1) % process_count
  declare can_print := true

  if process_number > 0 then
    receive(&can_print, 1, previous_process)
  end if
  print "Hello from main thread of process ", process_number, " of " \
    , process_count, " on ", process_hostname
  send(can_print, 1, next_process)
end procedure