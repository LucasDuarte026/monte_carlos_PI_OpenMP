#!/bin/bash

for var in {1..10}; do
    bash ./benchmarks/optimized_simd_bench.sh ${var}
    bash ./benchmarks/optimized_bench.sh ${var}
    bash ./benchmarks/parallel_reference_bench.sh ${var}
    bash ./benchmarks/schedule_auto_bench.sh ${var}
    bash ./benchmarks/schedule_dynamic_bench.sh ${var}
    bash ./benchmarks/schedule_guided_bench.sh ${var}
    bash ./benchmarks/sequencial_reference_bench.sh ${var}
done