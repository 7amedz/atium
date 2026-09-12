import json
import sys

import QuantLib as ql

CASES = [
    (0.99, 0.97, 0.5, "midpoint between declining positive factors"),
    (0.98, 1.02, 0.25, "inverted factors with an interior factor above the start"),
    (1.005, 0.995, 0.75, "factors straddling one under negative-rate-compatible inputs"),
]


def main() -> None:
    cases = []
    for start_factor, end_factor, interpolation_fraction, note in CASES:
        interpolation = ql.LogLinearInterpolation([0.0, 1.0], [start_factor, end_factor])
        cases.append(
            {
                "start_factor": start_factor,
                "end_factor": end_factor,
                "interpolation_fraction": interpolation_fraction,
                "note": note,
                "expected_factor": interpolation(interpolation_fraction),
            }
        )

    json.dump(
        {
            "source": f"QuantLib {ql.__version__}",
            "interpolation": "LogLinearInterpolation on coordinates [0.0, 1.0] without extrapolation",
            "factor_contract": "strictly positive finite factors; factors above one and inverted pairs are valid",
            "tolerance": {"kind": "relative", "value": 1e-12},
            "cases": cases,
        },
        sys.stdout,
        indent=2,
    )
    sys.stdout.write("\n")


if __name__ == "__main__":
    main()
