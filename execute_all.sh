#!/bin/bash

bash ./benchmarks/optimized_bench.sh
bash ./benchmarks/parallel_reference_bench.sh
bash ./benchmarks/schedule_auto_bench.sh
bash ./benchmarks/schedule_dynamic_bench.sh
bash ./benchmarks/schedule_guided_bench.sh
bash ./benchmarks/sequencial_reference_bench.sh