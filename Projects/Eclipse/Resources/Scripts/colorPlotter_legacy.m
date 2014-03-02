(* QBill plotter *)

Clear[Evaluate[Context[] <> "*"]]

baseDir = Directory[];

SetDirectory[baseDir <> "/W=" <> $ScriptCommandLine[[2]] <> "/out/rawColor/"];
files = FileNames[];
colorFiles = {};
phis = {};
For[i=1, i<=Length[files], i++,
	If[FileExtension[files[[i]]] == "bmp",
		AppendTo[colorFiles, Directory[] <> "/" <> files[[i]]];

		(* Obtejer valor de phi *)
		begin = StringPosition[files[[i]], "_"][[1]][[1]];
		end = Last[StringPosition[files[[i]], "."]][[1]];
		ph = ToExpression[StringTake[files[[i]], {begin + 1, end - 1}]];
		AppendTo[phis, ph];
		,False
	]
]

SetDirectory[baseDir];
colorFiles = colorFiles[[Ordering[phis]]];
phis = Sort[phis];
For[i=1, i<=Length[colorFiles], i++,
	(* Escribir progreso *)
	progress = "Plotting color plot " <> ToString[i] <> "/" <> ToString[Length[colorFiles]];
	Print[progress];

	Data = Import[colorFiles[[i]]];
	label = "\[Phi] = " <> ToString[phis[[i]]];

	image = Show[Data, PlotLabel -> Style[label, 14], AxesLabel -> {Style["x", 13], Style["y", 13]}];
	Export["W=" <> $ScriptCommandLine[[2]] <> "/out/colorPlots/img" <> ToString[i] <>".png",image]
]