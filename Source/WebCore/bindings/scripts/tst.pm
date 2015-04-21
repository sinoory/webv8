

package Test;

sub parseExtendedAttributes
{
    print "===========\n";
    my $str = shift;
    $str =~ s/\[\s*(.*?)\s*\]/$1/g;
    print "str=$str\n";

    my %attrs = ();

    foreach my $value (split(/\s*,\s*/, $str)) {
        print "varlue=$value\n";
        (my $name, my $val) = split(/\s*=\s*/, $value, 2);
        print "name=$name,val=$val\n";

        # Attributes with no value are set to be true
        $val = 1 unless defined $val;
        $attrs{$name} = $val;
        die("Invalid extended attribute name: '$name'\n") if $name =~ /\s/;
    }

    return \%attrs;
}

sub test1
{
my $at="";
$at="[NotEnumerable, Conditional=WILL_REVEAL_EDGE_EVENTS";
parseExtendedAttributes($at);

$at="[Conditional=POINTER_LOCK";
parseExtendedAttributes($at);


$at="Conditional=POINTER_LOCK";
parseExtendedAttributes($at);
}

sub getFuncAttr{
    my $funs=shift;
    #my $res="\s*((?:\[[^]]*\] )?)((?:unsigned )?(?:int|short|(?:long )?long)|(?:[a-zA-Z0-9:]*))\s*([a-zA-Z0-9:]*)\s*\(\s*([a-zA-Z0-9:\s,=\[\]]*)";
    my $res ="\s*((?:[^\]]*)?)";

    $funs =~ /$res/ or die "Parsing error!\nSource:\n$funs\n)";
    my $methodExtendedAttributes = (defined($1) ? $1 : " "); chop($methodExtendedAttributes);
    print "methodExtendedAttributes=$methodExtendedAttributes,\n";

}

sub test2{
    #my $funs="   kCallWith=ScriptExecutionContext] static void revokeObjectURL(DOMString url);";
    my $funs="   [CallWith=ScriptExecutionContext] static void revokeObjectURL(DOMString url);";
    getFuncAttr($funs);

}
test2();


