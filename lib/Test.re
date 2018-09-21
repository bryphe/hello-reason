module ReasonReact = React;

module ReactDOMRe = React.ReactDOMRe;


/***
 * The simplest component. Composes nothing!
 */
module Box = {
  let component = ReasonReact.statefulComponent("Box");
  let make = (~title="ImABox", ~onClick as _=?, _children) => {
    ...component,
    initialState: () => (),
    render: (_self) => ReasonReact.stringToElement(title)
  };
};

module UpdateAlternateClicks = {
  type action =
    | Click;
  let component = ReasonReact.reducerComponent("UpdateAlternateClicks");
  let make = (~rAction, _children) => {
    ...component,
    initialState: () => 0,
    printState: (state) => string_of_int(state),
    reducer: (Click, state) => Update(state + 1),
    shouldUpdate: ({newSelf: {state}}) => state mod 2 === 0,
    render: ({state, send}) => {
      ReasonReact.RemoteAction.subscribe(~send, rAction);
      ReasonReact.stringToElement("Rendered state is " ++ string_of_int(state))
    }
  };
};

module TestUtils = {
  open React;
  let logMsg = (~msg) => logString("\n" ++ (msg ++ "\n-------------------"));
  let startTest = (~msg) => {
    GlobalState.reset();
    if (msg != "") {
      logString("\n\n******* " ++ (msg ++ " *******"))
    }
  };

  /*** Print a renderedElement */
  let printRenderedElement = (renderedElement: RenderedElement.t) =>
    OutputTree.print(OutputTree.fromRenderedElement(renderedElement));

  /*** Print a list of renderedElement */
  let printRenderedElementList = (el) =>
    printRenderedElement(RenderedElement.listToRenderedElement(el));
  let renderAndPrint = (~msg, reactElement) => {
    logMsg(~msg);
    let rendered = RenderedElement.render(reactElement);
    logString(printRenderedElement(rendered));
    rendered
  };
  let checkApplyUpdateLogs = (updateLog, renderedElement, newRenderedElement) => {
    let outputTree = OutputTree.fromRenderedElement(renderedElement);
    let newOutputTree = OutputTree.fromRenderedElement(newRenderedElement);
    let outputTreeAfterApply = OutputTree.applyUpdateLog(updateLog, outputTree);
    let newOutputTreeString = OutputTree.print(newOutputTree);
    let outputTreeAfterApplyString = OutputTree.print(outputTreeAfterApply);
    if (newOutputTreeString != outputTreeAfterApplyString) {
      logString("Tree expected after updateLog:");
      logString(newOutputTreeString);
      logString("Tree found:");
      logString(outputTreeAfterApplyString);
      assert false
    }
  };
  let updateAndPrint = (~msg, renderedElement, reactElement) => {
    logMsg(~msg);
    let (newRenderedElement, updateLog) = RenderedElement.update(renderedElement, reactElement);
    checkApplyUpdateLogs(updateLog, renderedElement, newRenderedElement);
    logString(printRenderedElement(newRenderedElement));
    newRenderedElement
  };
  let flushAndPrint = (~msg, renderedElement) => {
    logMsg(~msg);
    let (newRenderedElement, updateLog) = RenderedElement.flushPendingUpdates(renderedElement);
    checkApplyUpdateLogs(updateLog, renderedElement, newRenderedElement);
    logString(printRenderedElement(newRenderedElement));
    newRenderedElement
  };
  let printAll = (renderedElements) => {
    logMsg(~msg="Trees rendered");
    print_string(printRenderedElementList(renderedElements))
  };
};

module RunTest1 = {
  open TestUtils;
  startTest(~msg="Test 1");
  let rendered0 = renderAndPrint(~msg="BoxWrapper with one box", <Box />);
  let rendered1 =
    updateAndPrint(~msg="BoxWrapper with two boxes", rendered0, [<Box />, <Box />]);
  printAll([rendered0, rendered1]);
};