(* QBill color plotter *)

Clear[Evaluate[Context[] <> "*"]]

baseDir = Directory[];

SetDirectory[baseDir <> "/W=" <> $ScriptCommandLine[[2]] <> "/out/csv/"];
files = FileNames[];
csvFiles = {};
phis = {};
For[i=1, i<=Length[files], i++,
	If[FileExtension[files[[i]]] == "csv",
		AppendTo[csvFiles, Directory[] <> "/" <> files[[i]]];

		(* Obtejer valor de phi *)
		begin = StringPosition[files[[i]], "_"][[1]][[1]];
		end = Last[StringPosition[files[[i]], "."]][[1]];
		ph = ToExpression[StringTake[files[[i]], {begin + 1, end - 1}]];
		AppendTo[phis, ph];
		,False
	]
]

SetDirectory[baseDir];
csvFiles = csvFiles[[Ordering[phis]]];
phis = Sort[phis];
For[i=1, i<=Length[csvFiles], i++,
	(* Escribir progreso *)
	progress = "Plotting color file " <> ToString[i] <> "/" <> ToString[Length[csvFiles]];
	Print[progress];

	Data = Import[csvFiles[[i]]];

	scaledData = {};
	dMax = Max[Abs[Data[[All, 3]]]];
	For[j = 1, j < Length[Data], j++,
		AppendTo[scaledData, {Data[[j]][[1]], Data[[j]][[2]], Abs[Data[[j]][[3]]]/dMax}]
	];

	label = "\[Phi] = " <> ToString[phis[[i]]];
	image = ListDensityPlot[scaledData, PlotRange->Full, PlotLabel->Style[label, 14], AxesLabel->{Style["x", 13], Style["y", 13]}, 
							AxesOrigin -> {0, 0} , ColorFunction -> "Rainbow", PlotLegends -> Automatic, Mesh -> Automatic, BaseStyle -> {FontSize -> 14}];
	Export["W=" <> $ScriptCommandLine[[2]] <> "/out/nModesPlots/img" <> ToString[i] <>".png",image]
]