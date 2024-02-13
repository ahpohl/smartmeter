\c smartmeter

create or replace function get_electricity_trend(
 	out energy_percent float,
  out bill_percent float)
language plpgsql
as
$$
declare
begin
	drop table if exists temp_trend;
	
	-- initialize variables
	energy_percent := 0;
	bill_percent := 0;
	
	-- calculate last year energy
	create temp table temp_trend
	as
	select
		date_trunc('year', now() - interval '1' year) as time,
		sum(energy) as energy,
    sum(bill) as bill
	from daily_view
	where 
    time between date_trunc('year', now() - interval '1' year) and
      now() - interval '1' year - interval '1 day';

	-- calculate current year energy
	insert into temp_trend (time, energy, bill)
	select
		date_trunc('year', now()) as time,
		sum(energy) as energy,
		sum(bill) as bill
	from daily_view
	where
		time >= date_trunc('year', now());
	
	-- calculate percentages
  select (lead(energy) over (order by time) - energy) / nullif(energy, 0) * 100 from temp_trend into energy_percent;
	select (lead(bill) over (order by time) - bill) / nullif(bill, 0) * 100 from temp_trend into bill_percent;
end;
$$
