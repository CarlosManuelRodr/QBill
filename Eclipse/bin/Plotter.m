(* QBill plotter *)

SetDirectory[Directory[] <> "/out/"];
files = FileNames[];
csvFiles = {};
phis = {};
For[i=1, i<Length[files], i++,
	If[FileExtension[files[[i]]] == "csv",
		AppendTo[csvFiles, files[[i]]];

		(* Obtejer valor de phi *)
		begin = StringPosition[files[[i]], "_"][[1]][[1]];
		end = Last[StringPosition[files[[i]], "."]][[1]];
		ph = ToExpression[StringTake[files[[i]], {begin + 1, end - 1}]];
		AppendTo[phis, ph];
		,False
	]
]

csvFiles = csvFiles[[Ordering[phis]]];
phis = Sort[phis];
For[i=1, i<=Length[csvFiles], i++,
	(* Escribir progreso *)
	progress = "Plotting file " <> ToString[i] <> "/" <> ToString[Length[csvFiles]];
	Print[progress];

	Data = Import[csvFiles[[i]]];
	label = "\[Phi] = " <> ToString[phis[[i]]];
	image = ListPlot3D[Data,PlotRange->Full, PlotLabel->Style[label, 14], AxesLabel->{Style["x", 13], Style["y", 13], Style["a", 13]} ];
	Export["img/img" <>ToString[i]<>".png",image]
]